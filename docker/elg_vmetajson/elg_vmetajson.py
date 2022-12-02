#!/usr/bin/env python3

from subprocess import Popen, PIPE, STDOUT
from typing import Dict, List

class FS_IO_CONV:
    def elg_sentences_tokens_morf_2_fs_xml_morf(self, sentences: List, tokens: List) -> str:
        '''
        elg sõnestaja väljund morfi sisendstringiks
        @param sentences: lausete massiiv
        @param tokens: sõnede massiiv
        @return: <s> token1 token2 ... </s>\n...
        '''
        text = ''
        s = t = 0
        while s < len(sentences):
            assert(tokens[t].start == sentences[s].start)
            text += '<s>\n'
            while t < len(tokens) and tokens[t].end <= sentences[s].end:
                text += f' {self.text_2_xml(tokens[t].features["token"])}'
                for morph in tokens[t].features["morph"]:
                    text += f'    {morph["lemma"]} //_{morph["pos"]}_'
                    if morph["feature"] != '':
                        text += f' {morph["feature"]},'
                    text += ' //'
                t += 1
                text += '\n'
            text += '</s>\n'
            s += 1
        return text

    def elg_sentences_tokens_2_fs_xml_text(self, sentences: List, tokens: List) -> str:
        '''
        elg sõnestaja väljund morfi sisendstringiks
        @param sentences: lausete massiiv
        @param tokens: sõnede massiiv
        @return: <s> token1 token2 ... </s>\n...
        '''
        text = ''
        s = t = 0
        while s < len(sentences):
            assert(tokens[t].start == sentences[s].start)
            text += '<s>'
            while t < len(tokens) and tokens[t].end <= sentences[s].end:
                text += f' {self.text_2_xml(tokens[t].features["token"])}'
                t += 1
            text += ' </s>\n'
            s += 1
        return text

    def fs_xml_morph_2_elg_morph(self, mrfstr: str) -> Dict:
        '''
        morfi väljundstring elg-kõlbulikuks
        :param mrfstr: TEKSTISÕNE   LEMMA //_POS_ FEATURES, //[...    LEMMA //_POS_ FEATURES, //]
        :return: { "token":string, "morph": [{ "lemma":string, "pos": string, "features":string }, ...] }
        '''
        eof_wordform = mrfstr.find('    ')
        startpos = eof_wordform+4
        analyysid_lst = []
        # tsükkel üle analüüsivariantide
        while startpos < len(mrfstr):
            endpos = mrfstr.find('    ', startpos)
            if endpos == -1:
                endpos = len(mrfstr)
            analyysid_lst.append(self.morph1str_2_dct(mrfstr[startpos:endpos]))
            startpos = endpos+4

        wordform = self.xml_2_txt(mrfstr[:eof_wordform])
        return {"token": wordform, "morph": analyysid_lst}

    def text_2_xml(self, s: str) -> str:
        '''
        Teisendab: plain-text -> fs-xml
        @param s: sisse plain-text
        @return: tagasi fs-xml
        '''
        return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace(' ', '<space/>')

    def xml_2_txt(self, s: str) -> str:
        '''
        Teisendab: fs-xml kuju -> plain-text
        @param s: sisse fs-xml
        @return: tagasi plain-text
        '''
        return s.replace('<space/>', ' ').replace('&amp;', '&').replace('&lt;', '<').replace('&gt;', '>')

    def morph1str_2_dct(self, analyys_str: str) -> Dict:
        '''
        FS analüüs -> ELG DCT
        :param analyys_str: sisse "LEMMA //_POS_ FEATURES, //"
        :return: { "lemma":string, "pos": string, "feature":string }
        '''
        analyys_dct = {}
        eof_lemma = analyys_str.find(' ')
        analyys_dct["lemma"] = analyys_str[:eof_lemma]
        analyys_dct["pos"] = analyys_str[eof_lemma+4]
        if analyys_str[eof_lemma+7] == '/':
            analyys_dct["feature"] = ''  # vormi pole
        else:
            analyys_dct["feature"] = analyys_str[eof_lemma+7:len(analyys_str)-3]
            if analyys_dct["feature"][len(analyys_dct["feature"])-1] == ',':
                analyys_dct["feature"] = analyys_dct["feature"][:len(analyys_dct["feature"])-1]
        return analyys_dct

def filosoft_morph4elg(sentences: List, tokens: List, path='.') -> List:
    conv = FS_IO_CONV()
    text = conv.elg_sentences_tokens_2_fs_xml_text(sentences, tokens)
    p = Popen([path+'/vmeta', '--xml', '--path', path], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
    lines = p.communicate(input=bytes(text, 'utf-8'))[0].decode('utf-8').split('\n')
    # tükeldame morf analüüsi tulemuse püütoni sõnastikuks ja
    # lisame analüüsid algsele andmestikule
    l = s = t = 0
    while s < len(sentences):
        assert(lines[l] == '<s>')
        l += 1
        while l < len(lines) and t < len(tokens) and tokens[t].end <= sentences[s].end:
            m = conv.fs_xml_morph_2_elg_morph(lines[l])
            assert(m["token"] == tokens[t].features["token"])
            tokens[t].features = m
            t += 1
            l += 1
        assert(lines[l] == '</s>')
        l += 1
        s += 1
    return tokens