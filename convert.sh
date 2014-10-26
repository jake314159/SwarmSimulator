#mencoder "mf://*.png" -o movie.avi -ovc lavc 
mencoder "mf://*.png" -o movie264_flip.avi -ovc x264 -flip

#mencoder "mf://*.png" -o movie.mpeg -ovc x264 -of mpeg -ofps 24
