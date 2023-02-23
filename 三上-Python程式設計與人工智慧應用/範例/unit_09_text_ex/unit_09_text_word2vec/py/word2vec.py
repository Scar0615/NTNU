# 訓練 word2vec model
#

from gensim.models import word2vec
import logging

def main():
    logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
    sentences = word2vec.Text8Corpus('wiki_seg.txt')
    model = word2vec.Word2Vec(sentences, size=400)
    # Save our model.
    model.save('wiki.model.bin')
    # To load a model.
    # model = word2vec.Word2Vec.load('wiki.model.bin')

if __name__ == "__main__":
    main()

