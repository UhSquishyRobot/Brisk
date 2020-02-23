# This is a wild, off kilter attempt at creating my own parser for extremely simple english sentences.
# I don't understand the full scope of a parsing library, so for now I think I just want it to validate
# sentences of the form "the cat sat on the mat," "the frog ate" or similar.

grammar = %Q(indicator: an | a | the | your | his | her | my;
noun: dog car | frog | pig | whistle | mat;
verb: sat ate | mended | mentioned;
preposition: on | over | in | around | under;
conjunction: and | or;
indicated_noun: <indicator>(1) <noun>(1);
action: <indicated_noun>(1) <verb>(1);
prepositional_phrase: <prepositon>(1) <indicated_noun>(1))

class Parser
  attr_accessor :types

  def initialize()
    @types = {}
  end
  
  def generate_grammar(str)
    entries = split_entries(str)
    @types = get_types(entries)
  end

  def split_entries(str)
    str.split(";\n")
  end

  def get_types(entries)
    entries.map do |entry|
      entry.split(":")
    end.to_h
  end

  def organize_types(types)
    types.each do |k, v|
      
    end
  end

  def validate(sentence)
    
  end
end

parser = Parser.new()

parser.generate_grammar(grammar)

