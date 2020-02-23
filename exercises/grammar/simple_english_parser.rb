grammar = %Q(indicator: an | a | the | your | his | her | my;
noun: dog car | frog | pig | whistle | mat;
verb: sat ate | mended | mentioned;
preposition: on | over | in | around | under;
conjunction: and | or;
indicated_noun: <indicator>(1) <noun>(1);
action: <indicated_noun>(1) <verb>(1);
prepositional_phrase: <prepositon>(1); <indicated_noun>(1))

class Parser
  def initialize()
    @types = {}
  end
  
  def generate_grammar(str)
    entries = split_entries(str)
    types = get_types(entries)
    types
  end

  def split_entries(str)
    str.split(";\n")
  end

  def get_types(entries)
    entries.map do |entry|
      entry.split(":")
    end.to_h
  end

  def validate(sentence)
    
  end
end

parser = Parser.new()

parser.generate_grammar(grammar)
