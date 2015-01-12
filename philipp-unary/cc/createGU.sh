targetdata="/home/rizkivmaster/ws/TextonBoost/google/data"
TEXTONIZE=build/src/textonize
$TEXTONIZE $targetdata/msrc_filterbank.dat FilterBank 400
$TEXTONIZE $targetdata/msrc_color.dat Color 128
$TEXTONIZE $targetdata/msrc_hog_l.dat HoG 150 L
$TEXTONIZE $targetdata/msrc_location.dat Location 144
#boost
build/src/textonboost $targetdata/msrc.dat $targetdata/msrc_color.dat $targetdata/msrc_filterbank.dat $targetdata/msrc_hog_l.dat $targetdata/msrc_location.dat
#evaluate
build/src/evaluate $targetdata/msrc.dat $targetdata/msrc_color.dat $targetdata/msrc_filterbank.dat $targetdata/msrc_hog_l.dat $targetdata/msrc_location.dat /home/rizkivmaster/ws/TextonBoost/google/unaries
