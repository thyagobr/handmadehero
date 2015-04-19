Adding explanations here:

-- For Some Audio functions (SDL_QueueAudio and SDL_GetQueuedAudioSize), we need
   SDL 2.0.4. Ubuntu 14.04's version is 2.0.0. Just purge the local version,
   fetch SDL 2.0.4 from their Mercurial repo or from tarball file. Then ./configure,
   make && sudo make install.
