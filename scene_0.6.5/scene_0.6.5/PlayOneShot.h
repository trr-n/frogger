#pragma once

class PlayOneShot
{
public:
	Stopwatch elapsedTime;
	Audio sound;

	PlayOneShot() {}
	PlayOneShot(String path) {
		sound = Audio(path);
	}

	// Not yet
	double ElapsedTime() {
		return 0.0;
	}

	bool IsStarted() {
		return elapsedTime.isStarted();
	}

	bool IsNotStarted() {
		return !elapsedTime.isStarted();
	}

	void Play() {
		sound.playOneShot();
		elapsedTime.restart();
	}
};
