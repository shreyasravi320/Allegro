
import nltk
from nltk.tokenize import word_tokenize, sent_tokenize
from nltk.corpus import stopwords

main_pat = r"""
Main: {<CD>?(<JJ>|<JJR>|<JJS>|<RB>|<RBR>|<RBS>)*(<NN>|<NNP>|<NNS>|<NNPS>)?(<VB>|<VBZ>|<VBG>)?(<JJ>|<JJR>|<JJS>|<RB>|<RBR>|<RBS>)*(<IN>?(<JJ>|<JJR>|<JJS>|<RB>|<RBR>|<RBS>)*(<NN>|<NNP>|<NNS>|<NNPS>)+(<JJ>|<JJR>|<JJS>|<RB>|<RBR>|<RBS>)*)?}
"""

# Download nltk packages if not already downloaded
# nltk.download()

# Part of speech tagging -> using PunktSentenceTokenizer: a trainable machine learning tokenizer
from nltk.tokenize import PunktSentenceTokenizer
from nltk.corpus import state_union

# Pass training data: GWBush state of the union address from 2006
training_data = state_union.raw("2006-GWBush.txt")

def preprocess(text):
    # Split each word and remove filler words (stop words like and, an, a, the)
    stop_words = set(stopwords.words("english"))

    text = text.lower()
    words = word_tokenize(text)
    filtered_list = [word for word in words if word not in stop_words]
    filtered_sentence = ' '.join(filtered_list)
    # print(filtered_sentence)

    custom_tokenizer = PunktSentenceTokenizer(training_data)

    tokenized = custom_tokenizer.tokenize(text)
    return tokenized

# NLTK Part Of Speech Tags:
#     CC coordinating conjunction
#     CD cardinal digit
#     DT determiner
#     EX existential there (like: "there is" ... think of it like "there exists")
#     FW foreign word
#     IN preposition/subordinating conjunction
#     JJ adjective 'big'
#     JJR adjective, comparative 'bigger'
#     JJS adjective, superlative 'biggest'
#     LS list marker 1)
#     MD modal could, will
#     NN noun, singular 'desk'
#     NNS noun plural 'desks'
#     NNP proper noun, singular 'Harrison'
#     NNPS proper noun, plural 'Americans'
#     PDT predeterminer 'all the kids'
#     POS possessive ending parent's
#     PRP personal pronoun I, he, she
#     PRP$ possessive pronoun my, his, hers
#     RB adverb very, silently,
#     RBR adverb, comparative better
#     RBS adverb, superlative best
#     RP particle give up
#     TO to go 'to' the store.
#     UH interjection errrrrrrrm
#     VB verb, base form take
#     VBD verb, past tense took
#     VBG verb, gerund/present participle taking
#     VBN verb, past participle taken
#     VBP verb, sing. present, non-3d take
#     VBZ verb, 3rd person sing. present takes
#     WDT wh-determiner which
#     WP wh-pronoun who, what
#     WP$ possessive wh-pronoun whose
#     WRB wh-abverb where, when
bad_tags = ['EX', 'DT', 'CC', 'FW', 'UH']
bad_vals = ["'s", "'t", "n't", 'is', 'are']

def process_content(tokens):
    try:
        for token in tokens:
            words = word_tokenize(token)
            tagged = nltk.pos_tag(words)    # Part of Speech tagging (pos)

            # Store associated phrases in chunks using regex
            chunk_gram = main_pat
            chunk_parser = nltk.RegexpParser(chunk_gram)
            tagged = [tag for tag in tagged if tag[1] not in bad_tags and tag[0] not in bad_vals]
            chunked = chunk_parser.parse(tagged)
            # chunked.draw()

        return chunked

    except Exception as e:
        print(str(e))

import numpy as np

print('Processing data...')

# Category -> words
models = ['Objects', 'Animals', 'Vehicles', 'Environments']
materials = ['Colors', 'Textures', 'Properties']

data = {
    'Objects': ['ball', 'sphere', 'cube', 'pyramid', 'cone', 'football', 'table', 'bench', 'lamp', 'phone', 'trophy', 'tree', 'house', 'plant', 'stick', 'door', 'vase', 'paper', 'headphones', 'piano', 'chair', 'pencil',
                'eraser', 'backpack', 'bed', 'computer', 'rock', 'flag'],
    'Dimensions': ['big', 'small', 'tiny', 'large'],
    'Positions': ['foreground', 'front', 'middle', 'back', 'background', 'left', 'right', 'top', 'bottom', 'above', 'below', 'inside'],
    'Animals': ['bird', 'cat', 'dog', 'snake', 'butterfly'],
    'Vehicles': ['car', 'truck','ship', 'plane'],
    'Environments': ['ocean', 'desert', 'arctic', 'swamp', 'road', 'terrain', 'ground', 'sky'],
    'Colors': ['blue', 'brown', 'white', 'green'],
    'Textures': ['smooth', 'sandy', 'rough', 'muddy', 'wooden'],
    'Properties': ['running', 'jumping', 'bouncing', 'swimming', 'rolling', 'flying', 'flies', 'breathes', 'swims']
}

# Words -> category
categories = {word: key for key, words in data.items() for word in words}

# Load the whole embedding matrix
# embeddings_index = {}
# with open('glove.6B.300d.txt') as f:
#   for line in f:
#     values = line.split()
#     word = values[0]
#     embed = np.array(values[1:], dtype=np.float32)
#     embeddings_index[word] = embed
# # print('Loaded %s word vectors.' % len(embeddings_index))
# # Embeddings for available words
# data_embeddings = {key: value for key, value in embeddings_index.items() if key in categories.keys()}
# print('Finished processing data')

import pickle

# with open('embeddings_index.pickle', 'wb') as f:
#     pickle.dump(embeddings_index, f, protocol=pickle.HIGHEST_PROTOCOL)
#
# with open('data_embeddings.pickle', 'wb') as f:
#     pickle.dump(data_embeddings, f, protocol=pickle.HIGHEST_PROTOCOL)

with open('embeddings_index.pickle', 'rb') as f:
    embeddings_index = pickle.load(f)

with open('data_embeddings.pickle', 'rb') as f:
    data_embeddings = pickle.load(f)

print('Finished processing data')

# Use operator to get the max value from the dictionary
import operator

# Processing the query
def categorize(query):
    if query in categories:
        return categories[query]

    query_embed = embeddings_index[query]
    scores = {}
    for word, embed in data_embeddings.items():
        category = categories[word]
        dist = query_embed.dot(embed)
        dist /= len(data[category])
        scores[category] = scores.get(category, 0) + dist
    return max(scores.items(), key=operator.itemgetter(1))[0]

def get_dict(tokens):
    val = str(tokens)

    val = val.replace('\n ', '')
    val = val.replace('(', '')
    val = val.replace(')', '')
    val = val.replace('/', ': ')
    val = val.replace('Main', '')
    val = val[2:]

    ls = val.split(' ')
    ls = [l for l in ls if l != '']

    d = {}
    for x in range(len(ls) - 1):
        if x % 2 == 0:
            d[ls[x][:-1]] = ls[x + 1]

    return d

def get_animation_params(map):
    params = {}
    subj = ''
    obj = ''
    desc1 = []
    desc2 = []

    dict1 = {}
    dict2 = {}

    dict1_keys = []
    dict2_keys = []

    for key, value in map.items():
        if value == 'IN':
            break
        dict1_keys.append(key)
        # dict1[key] = map.pop(key)

    for key in dict1_keys:
        dict1[key] = map.pop(key)

    for key, value in map.items():
        dict2[key] = map[key]

    if dict1:
        for key, value in dict1.items():
            if value == 'NN':
                subj = key
            else:
                desc1.append(key)
        params[subj] = desc1


    if dict2:
        for key, value in dict2.items():
            if value == 'NN':
                obj = key
            else:
                desc2.append(key)
        params[obj] = desc2

    return params


import subprocess
from subprocess import Popen, PIPE
from nltk.stem import PorterStemmer

ps = PorterStemmer()

subprocess.call(['make'])

scene = str(input('Type: '))
objs = []

while scene:
    obj_cat = {}

    p = Popen(['./Allegro'], shell=True, stdout=PIPE, stdin=PIPE)

    tokenized = preprocess(scene)
    tokens = process_content(tokenized)

    map = get_dict(tokens)

    for key, value in map.items():
        c_key = categorize(key)
        if c_key == 'Colors':
            map[key] = 'JJ'
        if c_key == 'Positions':
            map[key] = 'JJ'
        if c_key == 'Textures':
            map[key] = 'JJ'
        if c_key == 'Properties':
            map[key] = 'VBZ'
    #     if value == 'NN' or value == 'NNS' or value == 'NNP' or value == 'NNPS':
    #         if c_key == 'Colors':
    #             c_key = 'Positions'
    #         obj_cat[key] = c_key
    #     else:
    #         obj_cat[key] = c_key

    map_copy = map.copy()
    for key, value in map_copy.items():
        map_copy[key] = categorize(key)

    keywrds = get_animation_params(map)
    # print(obj_cat)
    print('Map: ', map_copy)
    print('Keywords: ', keywrds)

    # for kk, vv in keywrds.items():
    #     for word in vv:
    #         print(ps.stem(word))

    # objs.append(obj_cat)

    size_k = 0
    size_v = 0

    for kk, vv in keywrds.items():
        if kk:
            size_k = 1
            size_v = len(vv)

    # Pass information from python to c++
    length_k = str(size_k) + '\n'
    length_k = bytes(length_k, 'UTF-8')
    p.stdin.write(length_k)

    length_v = str(size_v) + '\n'
    length_v = bytes(length_v, 'UTF-8')
    p.stdin.write(length_v)

    for kk, vv in keywrds.items():
        key_to_send = str(kk) + '\n'
        key_to_send = bytes(key_to_send, 'UTF-8')
        p.stdin.write(key_to_send)

        for vvv in vv:
            val_to_send = str(vvv) + '\n'
            val_to_send = bytes(val_to_send, 'UTF-8')
            p.stdin.write(val_to_send)

            desc_to_send = str(map_copy[vvv]) + '\n'
            desc_to_send = bytes(desc_to_send, 'UTF-8')
            p.stdin.write(desc_to_send)

    p.stdin.flush()
    line = p.stdout.readline()
    while line:
        result = line.strip().decode()
        print(result)
        line = p.stdout.readline()

    # for obj in objs:
    #     for key, val in obj.items():
    #         cat_key = val
    #
    #         if val in models:
    #             cat_key = 'Model'
    #
    #         param1 = str(cat_key) + '\n'
    #         param1 = bytes(param1, 'UTF-8')  # Needed in Python 3.
    #         p.stdin.write(param1)
    #
    #         param2 = str(key) + '\n'
    #         param2 = bytes(param2, 'UTF-8')  # Needed in Python 3.
    #         p.stdin.write(param2)
    #
    # p.stdin.flush()
    #
    # line = p.stdout.readline()
    #
    # while line:
    #     result = line.strip().decode()
    #     print(result)
    #     line = p.stdout.readline()

    scene = str(input('Type: '))

subprocess.call(['make', 'clean'])
