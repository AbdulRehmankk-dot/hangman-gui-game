#include "MainWindow.h"
#include <QApplication>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_gameActive(false)
{
    setupUI();
    setWindowTitle("Hangman Game");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title label
    QLabel* titleLabel = new QLabel("HANGMAN GAME", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2c3e50; padding: 10px;");
    m_mainLayout->addWidget(titleLabel);

    createGameArea();
    createControlArea();
    createLetterButtons();

    // Initially disable game controls
    enableGameControls(false);
}

void MainWindow::createGameArea()
{
    QGroupBox* gameGroup = new QGroupBox("Game Display", this);
    QVBoxLayout* gameLayout = new QVBoxLayout(gameGroup);

    // Hangman drawing display
    m_hangmanLabel = new QLabel(this);
    QFont monoFont("Courier New", 10);
    m_hangmanLabel->setFont(monoFont);
    m_hangmanLabel->setAlignment(Qt::AlignCenter);
    m_hangmanLabel->setStyleSheet("background-color: #ecf0f1; padding: 10px; border-radius: 5px;");
    m_hangmanLabel->setText(m_game.getHangmanDrawing());
    gameLayout->addWidget(m_hangmanLabel);

    // Word progress display
    m_wordProgressLabel = new QLabel("_ _ _ _ _", this);
    QFont wordFont = m_wordProgressLabel->font();
    wordFont.setPointSize(18);
    wordFont.setBold(true);
    wordFont.setLetterSpacing(QFont::AbsoluteSpacing, 5);
    m_wordProgressLabel->setFont(wordFont);
    m_wordProgressLabel->setAlignment(Qt::AlignCenter);
    m_wordProgressLabel->setStyleSheet("color: #16a085; padding: 15px;");
    gameLayout->addWidget(m_wordProgressLabel);

    // Remaining tries
    m_triesLabel = new QLabel("Remaining Tries: 7", this);
    QFont triesFont = m_triesLabel->font();
    triesFont.setPointSize(12);
    m_triesLabel->setFont(triesFont);
    m_triesLabel->setAlignment(Qt::AlignCenter);
    m_triesLabel->setStyleSheet("color: #27ae60; padding: 5px;");
    gameLayout->addWidget(m_triesLabel);

    // Guessed letters display
    m_guessedLettersLabel = new QLabel("Guessed Letters: None", this);
    m_guessedLettersLabel->setAlignment(Qt::AlignCenter);
    m_guessedLettersLabel->setStyleSheet("color: #7f8c8d; padding: 5px;");
    gameLayout->addWidget(m_guessedLettersLabel);

    // Status label
    m_statusLabel = new QLabel("Press 'Start Game' to begin!", this);
    QFont statusFont = m_statusLabel->font();
    statusFont.setPointSize(11);
    statusFont.setItalic(true);
    m_statusLabel->setFont(statusFont);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #3498db; padding: 10px;");
    gameLayout->addWidget(m_statusLabel);

    m_mainLayout->addWidget(gameGroup);
}

void MainWindow::createControlArea()
{
    QGroupBox* controlGroup = new QGroupBox("Game Controls", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    // Theme selection
    QHBoxLayout* themeLayout = new QHBoxLayout();
    QLabel* themeLabel = new QLabel("Select Theme:", this);
    themeLayout->addWidget(themeLabel);

    m_themeComboBox = new QComboBox(this);
    m_themeComboBox->addItem("Animals", static_cast<int>(HangmanGame::Theme::Animals));
    m_themeComboBox->addItem("Countries", static_cast<int>(HangmanGame::Theme::Countries));
    m_themeComboBox->addItem("Fruits", static_cast<int>(HangmanGame::Theme::Fruits));
    m_themeComboBox->addItem("Sports", static_cast<int>(HangmanGame::Theme::Sports));
    m_themeComboBox->addItem("Colors", static_cast<int>(HangmanGame::Theme::Colors));
    themeLayout->addWidget(m_themeComboBox);
    themeLayout->addStretch();

    controlLayout->addLayout(themeLayout);

    // Letter input area
    QHBoxLayout* inputLayout = new QHBoxLayout();
    QLabel* inputLabel = new QLabel("Guess a letter:", this);
    inputLayout->addWidget(inputLabel);

    m_letterInput = new QLineEdit(this);
    m_letterInput->setMaxLength(1);
    m_letterInput->setPlaceholderText("A-Z");
    m_letterInput->setAlignment(Qt::AlignCenter);
    QFont inputFont = m_letterInput->font();
    inputFont.setPointSize(14);
    m_letterInput->setFont(inputFont);
    inputLayout->addWidget(m_letterInput);

    m_guessButton = new QPushButton("Guess", this);
    m_guessButton->setStyleSheet("background-color: #3498db; color: white; padding: 8px 20px; font-weight: bold;");
    inputLayout->addWidget(m_guessButton);

    // Connect guess button signal
    connect(m_guessButton, &QPushButton::clicked, this, &MainWindow::onGuessLetter);
    // Connect return key in line edit
    connect(m_letterInput, &QLineEdit::returnPressed, this, &MainWindow::onGuessLetter);

    controlLayout->addLayout(inputLayout);

    // Main action buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_startButton = new QPushButton("Start Game", this);
    m_startButton->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 20px; font-weight: bold; font-size: 12pt;");
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
    buttonLayout->addWidget(m_startButton);

    m_scoresButton = new QPushButton("Check Scores", this);
    m_scoresButton->setStyleSheet("background-color: #f39c12; color: white; padding: 10px 20px; font-weight: bold; font-size: 12pt;");
    connect(m_scoresButton, &QPushButton::clicked, this, &MainWindow::onCheckScores);
    buttonLayout->addWidget(m_scoresButton);

    m_exitButton = new QPushButton("Exit", this);
    m_exitButton->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px 20px; font-weight: bold; font-size: 12pt;");
    connect(m_exitButton, &QPushButton::clicked, this, &MainWindow::onExit);
    buttonLayout->addWidget(m_exitButton);

    controlLayout->addLayout(buttonLayout);

    m_mainLayout->addWidget(controlGroup);
}

void MainWindow::createLetterButtons()
{
    QGroupBox* letterGroup = new QGroupBox("Quick Letter Selection", this);
    QVBoxLayout* letterLayout = new QVBoxLayout(letterGroup);

    m_letterButtonsWidget = new QWidget(this);
    QGridLayout* gridLayout = new QGridLayout(m_letterButtonsWidget);
    gridLayout->setSpacing(5);

    // Create A-Z buttons
    const QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int row = 0, col = 0;
    const int cols = 9; // 9 buttons per row

    for (QChar letter : alphabet) {
        QPushButton* btn = new QPushButton(QString(letter), this);
        btn->setFixedSize(40, 40);
        btn->setProperty("letter", letter);
        btn->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; font-weight: bold; }"
                           "QPushButton:hover { background-color: #7f8c8d; }"
                           "QPushButton:disabled { background-color: #bdc3c7; color: #7f8c8d; }");

        // Connect signal
        connect(btn, &QPushButton::clicked, this, &MainWindow::onLetterButtonClicked);

        gridLayout->addWidget(btn, row, col);
        m_letterButtons.append(btn);

        col++;
        if (col >= cols) {
            col = 0;
            row++;
        }
    }

    letterLayout->addWidget(m_letterButtonsWidget);
    m_mainLayout->addWidget(letterGroup);
}

void MainWindow::onStartGame()
{
    // Get selected theme
    int themeIndex = m_themeComboBox->currentData().toInt();
    HangmanGame::Theme theme = static_cast<HangmanGame::Theme>(themeIndex);

    // Start new game
    m_game.startNewGame(theme);
    m_gameActive = true;

    // Enable game controls
    enableGameControls(true);

    // Update display
    updateDisplay();
    m_statusLabel->setText("Game started! Guess the word!");
    m_letterInput->setFocus();
}

void MainWindow::onCheckScores()
{
    QStringList scores = m_game.loadScores();

    // Create dialog to display scores
    QDialog* scoreDialog = new QDialog(this);
    scoreDialog->setWindowTitle("High Scores");
    scoreDialog->resize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(scoreDialog);

    QLabel* titleLabel = new QLabel("Hall of Fame", scoreDialog);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QTextEdit* scoreText = new QTextEdit(scoreDialog);
    scoreText->setReadOnly(true);

    if (scores.isEmpty()) {
        scoreText->setPlainText("No scores recorded yet.\nBe the first to play!");
    } else {
        scoreText->setPlainText(scores.join("\n"));
    }

    layout->addWidget(scoreText);

    QPushButton* closeButton = new QPushButton("Close", scoreDialog);
    closeButton->setStyleSheet("background-color: #3498db; color: white; padding: 8px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, scoreDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    scoreDialog->exec();
}

void MainWindow::onExit()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Game",
                                  "Are you sure you want to exit?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }
}

void MainWindow::onGuessLetter()
{
    if (!m_gameActive) {
        return;
    }

    QString input = m_letterInput->text().trimmed().toUpper();

    // Validate input
    if (input.isEmpty()) {
        m_statusLabel->setText("Please enter a letter!");
        return;
    }

    if (!input[0].isLetter()) {
        m_statusLabel->setText("Only letters (A-Z) are allowed!");
        m_letterInput->clear();
        return;
    }

    QChar letter = input[0];

    // Make the guess
    bool found = m_game.guessLetter(letter);

    // Disable corresponding button
    for (QPushButton* btn : m_letterButtons) {
        if (btn->property("letter").toChar() == letter) {
            btn->setEnabled(false);
            break;
        }
    }

    // Clear input
    m_letterInput->clear();

    // Update display
    updateDisplay();

    // Update status message
    if (found) {
        m_statusLabel->setText(QString("Good guess! '%1' is in the word!").arg(letter));
    } else {
        m_statusLabel->setText(QString("Sorry! '%1' is not in the word.").arg(letter));

        // Check for hint
        if (m_game.getRemainingTries() == 2) {
            m_statusLabel->setText(m_statusLabel->text() + " HINT APPLIED!");
        }
    }

    // Check if game is over
    if (m_game.isGameOver()) {
        endGame();
    }
}

void MainWindow::onLetterButtonClicked()
{
    if (!m_gameActive) {
        return;
    }

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QChar letter = btn->property("letter").toChar();
    m_letterInput->setText(QString(letter));
    onGuessLetter();
}

void MainWindow::updateDisplay()
{
    // Update hangman drawing
    m_hangmanLabel->setText(m_game.getHangmanDrawing());

    // Update word progress
    m_wordProgressLabel->setText(m_game.getCurrentProgress());

    // Update remaining tries with color coding
    int tries = m_game.getRemainingTries();
    m_triesLabel->setText(QString("Remaining Tries: %1").arg(tries));

    if (tries <= 2) {
        m_triesLabel->setStyleSheet("color: #e74c3c; padding: 5px; font-weight: bold;");
    } else if (tries <= 4) {
        m_triesLabel->setStyleSheet("color: #f39c12; padding: 5px; font-weight: bold;");
    } else {
        m_triesLabel->setStyleSheet("color: #27ae60; padding: 5px;");
    }

    // Update guessed letters
    QString guessed = m_game.getGuessedLetters();
    if (guessed.isEmpty()) {
        m_guessedLettersLabel->setText("Guessed Letters: None");
    } else {
        m_guessedLettersLabel->setText(QString("Guessed Letters: %1").arg(guessed));
    }
}

void MainWindow::endGame()
{
    m_gameActive = false;
    enableGameControls(false);

    QString message;
    QString title;

    if (m_game.isGameWon()) {
        int score = m_game.getRemainingTries();
        title = "Congratulations!";
        message = QString("You won! The word was: %1\nYour score: %2 out of 7")
                      .arg(m_game.getSecretWord().toUpper())
                      .arg(score);

        m_statusLabel->setText("🎉 YOU WON! 🎉");
        m_statusLabel->setStyleSheet("color: #27ae60; padding: 10px; font-weight: bold; font-size: 12pt;");

        // Get player name and save score
        bool ok;
        QString playerName = QInputDialog::getText(this, "Enter Your Name",
                                                   "Congratulations! Enter your name for the scoreboard:",
                                                   QLineEdit::Normal, "", &ok);

        if (ok && !playerName.isEmpty()) {
            m_game.saveScore(playerName, score);
            QMessageBox::information(this, "Score Saved",
                                     QString("Your score has been saved!\n%1 - Score: %2/7")
                                         .arg(playerName).arg(score));
        }

    } else {
        title = "Game Over";
        message = QString("You lost! The word was: %1")
                      .arg(m_game.getSecretWord().toUpper());

        m_statusLabel->setText("☹️ GAME OVER - You ran out of tries!");
        m_statusLabel->setStyleSheet("color: #e74c3c; padding: 10px; font-weight: bold; font-size: 12pt;");
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::enableGameControls(bool enable)
{
    m_letterInput->setEnabled(enable);
    m_guessButton->setEnabled(enable);
    m_themeComboBox->setEnabled(!enable); // Disable theme selection during game

    // Enable/disable letter buttons
    for (QPushButton* btn : m_letterButtons) {
        btn->setEnabled(enable);
    }

    if (!enable) {
        m_letterInput->clear();
    }
}
