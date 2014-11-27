font=$1
pointsize=72
for letter in {a..z} {A..Z} {0..9} '%'; do
    convert -font "$font" -pointsize "$pointsize" label:"$letter" "$letter".jpg
    convert "$letter".jpg "$letter".bmp
done
