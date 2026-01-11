#ifndef HANGMANGAME_H
#define HANGMANGAME_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

/**
 * @brief The HangmanGame class encapsulates all game logic
 * Separated from UI for clean architecture
 */
class HangmanGame
{
public:
    enum class Theme {
        Animals,
        Countries,
        Fruits,
        Sports,
        Colors
    };

    HangmanGame();

    // Game control
    void startNewGame(Theme theme);
    bool guessLetter(QChar letter);
    bool isGameOver() const;
    bool isGameWon() const;

    // Game state
    QString getCurrentProgress() const;
    QString getHangmanDrawing() const;
    int getRemainingTries() const;
    int getMaxTries() const { return 7; }
    QString getGuessedLetters() const;
    QString getSecretWord() const { return m_secretWord; }

    // Score management
    void saveScore(const QString& playerName, int score);
    QStringList loadScores() const;

private:
    void initializeWordLists();
    QString selectRandomWord(Theme theme);
    void applyHint();

    QMap<Theme, QStringList> m_wordLists;
    QString m_secretWord;
    QString m_currentProgress;
    QVector<QChar> m_guessedLetters;
    int m_remainingTries;
    bool m_hintUsed;

    static const QString SCORES_FILE;
};

#endif // HANGMANGAME_H
