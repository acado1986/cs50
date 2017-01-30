import crypt
import sys
import argparse
import itertools
from pathlib import Path
import time

def main():
    
    # running time mesurements
    start_time = time.time()
    
    # path to default dictionary in Ubuntu distros
    default_dictionary = '/usr/share/dict/american-english'
    
    # ensure correct usage of the program
    parser= argparse.ArgumentParser()
    parser.add_argument("dictionary", default=default_dictionary, nargs="?", help="path to different dictionary, else default")
    parser.add_argument("chiphertext",help="encrypted text to decipher")
    args = parser.parse_args()
    
    # store supplied dictionary or default
    dictionary = args.dictionary
    
    # store file or encrypted text
    encrypted_text= args.chiphertext 
    
    # decrypt using a dictionary
    def decryptDictionary(encrypted_text, dictionary):
        ''' The function reads dictionary in a text format, arranged one 
        word by line, encrypts the word and compares it to the encrypted text''' 
        salt = encrypted_text[:2]
        with open(dictionary, 'r') as dictionary:
            for word in dictionary:
                # pset6 specificacion only password only 4 letters long
                if len(word) < 5:
                    if encrypted_text == crypt.crypt(word.rstrip('\n'), salt):
                        return word.rstrip('\n')
                        
            else:
                return None
                
     # decrypt using brute force         
    def decryptBruteForce(encrypted_text, wordLength):
        ''' The function generates a permutation of letters to form words,
        of wordLength length, encrypts the word and compares it to the encrypted text'''
        salt = encrypted_text[:2]
        chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
        for word in (itertools.product(chars, repeat=wordLength)):
            if encrypted_text == crypt.crypt(''.join(word), salt):
                        return ''.join(word)
        else:
            return None
            
    # find the password first in dictionary, if unsuccesfull use brute force
    dict_passwd = decryptDictionary(encrypted_text, dictionary)
    if dict_passwd == None:
        # password max lenght only 4 letters
        for i in range(5):
            brute_passwd = decryptBruteForce(encrypted_text, i)
            if brute_passwd is not None:
                break
    print('{}'.format(dict_passwd if dict_passwd is not None else brute_passwd))    
    
    # print running time
    end_time = time.time()
    print("--- %s seconds ---" % (end_time - start_time))
    
if __name__ == '__main__':
    main()