#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QFont>
#include "HangmanGame.h"

/**
 * @brief The MainWindow class handles all UI interactions
 * Connects to HangmanGame logic via signals and direct calls
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Button handlers
    void onStartGame();
    void onCheckScores();
    void onExit();
    void onGuessLetter();
    void onLetterButtonClicked();

private:
    // UI setup methods
    void setupUI();
    void createMenuBar();
    void createGameArea();
    void createControlArea();
    void createLetterButtons();

    // Game update methods
    void updateDisplay();
    void resetGame();
    void endGame();
    void enableGameControls(bool enable);

    // UI Components
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;

    // Display area
    QLabel* m_hangmanLabel;
    QLabel* m_wordProgressLabel;
    QLabel* m_triesLabel;
    QLabel* m_guessedLettersLabel;
    QLabel* m_statusLabel;

    // Control area
    QComboBox* m_themeComboBox;
    QPushButton* m_startButton;
    QPushButton* m_scoresButton;
    QPushButton* m_exitButton;

    // Input area
    QLineEdit* m_letterInput;
    QPushButton* m_guessButton;
    QWidget* m_letterButtonsWidget;
    QVector<QPushButton*> m_letterButtons;

    // Game logic
    HangmanGame m_game;
    bool m_gameActive;
};

#endif // MAINWINDOW_H
