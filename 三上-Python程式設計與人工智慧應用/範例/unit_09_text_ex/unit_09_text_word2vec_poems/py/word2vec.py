# 訓練 word2vec model
#

from gensim.models import word2vec
import logging

def main():
    logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
    sentences = word2vec.Text8Corpus('c:/python/corpus/corpus_cna_xin_wiki_cis180.txt')
    model = word2vec.Word2Vec(sentences, size=256)
    # Save our model.
    model.save('c:/python/w2v_model/cna_xin_wiki_cis180_256.model.bin')
    # To load a model.
    # model = word2vec.Word2Vec.load('c:/python/w2v_model/cna_xin_wiki_cis180_256.model.bin')

if __name__ == "__main__":
    main()
