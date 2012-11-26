#!/usr/bin/env ruby

# "Usage" Erklärung und Suche nach möglichem Dateipfad zu einer Scoringsmatrix. 
def checkCommandline
  usage = "Usage: #{$0} (optional Matrix file, default: BLOSUM62)"
  if ARGV.length == 0
    STDERR.puts "#{usage}"
    puts "No Matrix file specified. Script will use the default BLOSUM62."
    return "default"
  elsif ARGV.length == 1
    return ARGV[0]
  else
    STDERR.puts "#{usage}"
    exit 1
  end
end    

# Testen und einlesen der angegebenen Datei. Ohne spezielle Angabe wird die 
# BLOSUM62 Matrix benutzt.
def readFile(path)
  lines=[]
  if path == "default"
    path = "/projects/lehre/gsa/BLOSUM62"
#    path = "./BLOSUM62"
  end
  
  begin
    file = File.new(path,"r")
    file.each_line do |line|
      lines.push line
    end
    file.close
  rescue => err
    puts "Exception: #{err}"
    err
  end
  return lines
end

# Parsen der Matrix in Hash of Hashes Dateiformat. Aminosaeure codes werden als
# hashkeys benutzt.
def parseMatrix(lines)
  begin
  scoreMatrix = Hash.new()
  colnames,scoreset=[]

    lines.each do |line|
      if line.match /^\s{1}/              # Lese Column Namen aus Zeile 1
        colnames=line.gsub(/\s+/, "").split(//)
      else
      
        unless colnames.empty? then       
          scoreset=line.rstrip.split(/ /)
          scoreset.reject! { |s| s.empty? }
          startAS=scoreset.shift          # Erstes Element als Start Aminosäure
          scoreMatrix[startAS] = Hash.new() # Erstelle neue Hash für jede AS
            
            if colnames.length == scoreset.length
              colnames.each_with_index do |column, index|
                scoreMatrix[startAS][column] = scoreset[index] # Fülle Hashs           
              end
            else
              puts "Matrix formatting not recognized."
            end   
        else
          puts "Matrix formatting not recognized."
        end
        
      end
    end
    
  rescue => err
    puts "Exception: #{err}"
    err
  end
  return scoreMatrix
end

# Koordinaten basierte Zugriffsfunktion für die Matrix
def getScore(scoreMatrix, x, y)
  x.upcase!
  y.upcase!
  if x =~ /[A-Z]{1}/
    if y =~ /[A-Z]{1}/
      if [x, y].any? {|k| scoreMatrix.key?(k)}
        puts "#{scoreMatrix[x][y]}"
    #   return scoreMatrix[x][y]
      else
        puts "Invalid aminoacid coding. Letter not in matrix."
        exit 1
      end
    else
      puts "Invalid single letter aminoacid coding."
      exit 1
    end  
  else
    puts "Invalid single letter aminoacid coding."
    exit 1
  end
end

# Wird das script direkt aufgerufen, lade die Matrix
# und gebe einen Beispielwert aus
if __FILE__ == $0
    scoreMatrix = parseMatrix(readFile(checkCommandline()))
    getScore(scoreMatrix, "a", "x")
end
