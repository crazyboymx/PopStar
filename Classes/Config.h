/*
 * Config.h
 *
 *  Created on: 2013-7-25
 *      Author: maxing
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

class Config {
public:
	enum GameMode {
		Normal = 0,
		Challenge = 1,
		Learning = 2,
	};

	virtual ~Config();
	static Config* instance();

	void load();
	void flush();

	bool mute() { return m_mute; }
	void setMute(bool mute);
	bool hasSavedPuzzle() { return m_hasSavedPuzzle; }
	GameMode savedPuzzleMode() { return m_savedPuzzleMode; }
	void setSavedPuzzleMode(GameMode mode) { m_savedPuzzleMode = mode; }
	int highScore() { return m_highScore; }
	void updateHighScore(int score) {
		if (m_highScore < score)
			m_highScore = score;
	}
	int highLevel() { return m_highScore; }
	void updateHighLevel(int level) {
		if (m_highLevel < level)
			m_highLevel = level;
	}
	void clearSavedPuzzle();
	void savePuzzle(std::string puzzle, int row, int col, int level, int score, int target);
	int score()     { return m_savedScore;  }
	int target()	{ return m_savedTarget; }
	int level()     { return m_savedLevel;  }
	int row()       { return m_savedRow;    }
	int col()       { return m_savedCol;    }
	std::string puzzle()   { return m_savedPuzzle; }
private:
	Config();
	Config(const Config&) {};
	static Config* m_instance;

	bool m_mute;
	bool m_hasSavedPuzzle;
	GameMode m_savedPuzzleMode;
	int  m_highScore;
	int  m_highLevel;
	int  m_savedScore;
	int  m_savedTarget;
	int  m_savedLevel;
	int  m_savedRow;
	int  m_savedCol;
	std::string m_savedPuzzle;
};

#endif /* CONFIG_H_ */
