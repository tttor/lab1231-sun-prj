tgtdir = 'boundingboxes/';
[modelsdirs,srcdirs] = textread('extendedconcate','%s %s');
extractBoundaries(modelsdirs,srcdirs,tgtdir)