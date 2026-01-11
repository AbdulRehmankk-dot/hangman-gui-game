#include "HangmanGame.h"

const QString HangmanGame::SCORES_FILE = "scores.txt";

HangmanGame::HangmanGame()
    : m_remainingTries(7)
    , m_hintUsed(false)
{
    initializeWordLists();
}

void HangmanGame::initializeWordLists()
{
    // Animals theme
    m_wordLists[Theme::Animals] = QStringList{
        "elephant", "giraffe", "penguin", "dolphin", "kangaroo",
        "butterfly", "crocodile", "hippopotamus", "cheetah", "octopus"
    };

    // Countries theme
    m_wordLists[Theme::Countries] = QStringList{
        "australia", "brazil", "canada", "denmark", "egypt",
        "france", "germany", "india", "japan", "mexico"
    };

    // Fruits theme
    m_wordLists[Theme::Fruits] = QStringList{
        "apple", "banana", "cherry", "mango", "orange",
        "pineapple", "strawberry", "watermelon", "blueberry", "papaya"
    };

    // Sports theme
    m_wordLists[Theme::Sports] = QStringList{
        "football", "basketball", "tennis", "cricket", "volleyball",
        "baseball", "hockey", "badminton", "swimming", "athletics"
    };

    // Colors theme
    m_wordLists[Theme::Colors] = QStringList{
        "red", "blue", "green", "yellow", "purple",
        "orange", "pink", "brown", "black", "white"
    };
}

void HangmanGame::startNewGame(Theme theme)
{
    m_secretWord = selectRandomWord(theme);
    m_currentProgress = QString(m_secretWord.length(), '_');
    m_guessedLetters.clear();
    m_remainingTries = 7;
    m_hintUsed = false;
}

QString HangmanGame::selectRandomWord(Theme theme)
{
    const QStringList& words = m_wordLists[theme];
    if (words.isEmpty()) {
        return "hangman"; // Fallback
    }

    int index = QRandomGenerator::global()->bounded(words.size());
    return words[index].toLower();
}

bool HangmanGame::guessLetter(QChar letter)
{
    letter = letter.toLower();

    // Check if already guessed
    if (m_guessedLetters.contains(letter)) {
        return false; // Already guessed, don't penalize
    }

    m_guessedLetters.append(letter);

    // Check if letter is in the word
    bool found = false;
    for (int i = 0; i < m_secretWord.length(); ++i) {
        if (m_secretWord[i] == letter) {
            m_currentProgress[i] = letter;
            found = true;
        }
    }

    if (!found) {
        m_remainingTries--;

        // Auto-hint at 2 remaining tries
        if (m_remainingTries == 2 && !m_hintUsed) {
            applyHint();
        }
    }

    return found;
}

void HangmanGame::applyHint()
{
    m_hintUsed = true;

    // Find first unrevealed letter
    for (int i = 0; i < m_secretWord.length(); ++i) {
        if (m_currentProgress[i] == '_') {
            QChar hintLetter = m_secretWord[i];

            // Reveal all instances of this letter
            for (int j = 0; j < m_secretWord.length(); ++j) {
                if (m_secretWord[j] == hintLetter) {
                    m_currentProgress[j] = hintLetter;
                }
            }

            m_guessedLetters.append(hintLetter);
            break;
        }
    }
}

bool HangmanGame::isGameOver() const
{
    return m_remainingTries <= 0 || isGameWon();
}

bool HangmanGame::isGameWon() const
{
    return !m_currentProgress.contains('_');
}

QString HangmanGame::getCurrentProgress() const
{
    QString spaced;
    for (int i = 0; i < m_currentProgress.length(); ++i) {
        if (i > 0) spaced += " ";
        spaced += m_currentProgress[i];
    }
    return spaced;
}

QString HangmanGame::getHangmanDrawing() const
{
    int stage = 7 - m_remainingTries;

    QStringList drawings = {
        // Stage 0 - Empty
        "   ______\n"
        "   |    |\n"
        "   |\n"
        "   |\n"
        "   |\n"
        "   |\n"
        "  _|_\n",

        // Stage 1 - Head
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |\n"
        "   |\n"
        "   |\n"
        "  _|_\n",

        // Stage 2 - Body
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |    |\n"
        "   |\n"
        "   |\n"
        "  _|_\n",

        // Stage 3 - Left arm
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |   /|\n"
        "   |\n"
        "   |\n"
        "  _|_\n",

        // Stage 4 - Right arm
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |   /|\\\n"
        "   |\n"
        "   |\n"
        "  _|_\n",

        // Stage 5 - Left leg
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |   /|\\\n"
        "   |   /\n"
        "   |\n"
        "  _|_\n",

        // Stage 6 - Right leg (Dead)
        "   ______\n"
        "   |    |\n"
        "   |    O\n"
        "   |   /|\\\n"
        "   |   / \\\n"
        "   |\n"
        "  _|_\n"
    };

    if (stage >= 0 && stage < drawings.size()) {
        return drawings[stage];
    }
    return drawings[0];
}

int HangmanGame::getRemainingTries() const
{
    return m_remainingTries;
}

QString HangmanGame::getGuessedLetters() const
{
    QString result;
    for (const QChar& letter : m_guessedLetters) {
        if (!result.isEmpty()) result += ", ";
        result += letter.toUpper();
    }
    return result;
}

void HangmanGame::saveScore(const QString& playerName, int score)
{
    QFile file(SCORES_FILE);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Challenger " << playerName << " - Your score is: "
            << score << " out of 7\n";
        file.close();
    }
}

QStringList HangmanGame::loadScores() const
{
    QStringList scores;
    QFile file(SCORES_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                scores.append(line);
            }
        }
        file.close();
    }

    return scores;
}
