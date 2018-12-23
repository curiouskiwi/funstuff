#calculate the scrabble score
#brenda anderson
import sys
score = {"a": 1, "b": 3, "c": 3, "d": 2, "e": 1, "f": 4, 
         "g": 2, "h": 4, "i": 1, "j": 8, "k": 5, "l": 1, 
         "m": 3, "n": 1, "o": 1, "p": 3, "q": 10, "r": 1, 
         "s": 1, "t": 1, "u": 1, "v": 4, "w": 4, "x": 8, 
         "y": 4, "z": 10}

def scrabble_score(word):
    tally = 0
    for letter in word:
        for key in score:
            if key == letter.lower():
                #print "found it", score[key]
                tally += score[key]
    return tally

if len(sys.argv) is 2:
    word = sys.argv[1]
    print (str(word) + ' is worth ' + str(scrabble_score(word)) + ' points')
else:
    print ("usage: python3 scrabblescore.py word")