/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "codeeditor.h"

//![constructor]

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), c(0)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
//    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

    QFont font;
    font.setPointSize(12);
    font.setFamily("Microsoft YaHei");
    QFont lineNumberAreaFont;
    lineNumberAreaFont.setPointSize(12);
    lineNumberAreaFont.setFamily("Courier New");
    lineNumberArea->setFont(lineNumberAreaFont);
    setFont(font);

    updateLineNumberAreaWidth(0);
//    highlightCurrentLine();

    m_lineNumber = -1;

    QPalette cPalette = palette();
    cPalette.setColor(QPalette::Inactive, QPalette::Highlight,
                     cPalette.color(QPalette::Active, QPalette::Highlight));
    cPalette.setColor(QPalette::Inactive, QPalette::HighlightedText,
                     cPalette.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(cPalette);
}

//![constructor]

CodeEditor::~CodeEditor()
{
}

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

#if ( QT_VERSION < QT_VERSION_CHECK(5,11,0))
    int space = digits < 2 ? 15 + fontMetrics().width(QLatin1Char('9')) * 2
                           : 15 + fontMetrics().width(QLatin1Char('9')) * digits;
#else
    int space = digits < 2 ? 15 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * 2
                           : 15 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#endif

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(-5, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]

//! [2]
void CodeEditor::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}
//! [2]

//! [3]
QCompleter *CodeEditor::completer() const
{
    return c;
}
//! [3]

//! [4]
void CodeEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QString tempCompletion = completion;
    if(tempCompletion.contains("【"))
        tempCompletion = tempCompletion.mid(0, tempCompletion.indexOf("【"));
    if(tempCompletion.startsWith("%i") || tempCompletion.startsWith("%zt"))
        emit imageSelected(tempCompletion, c->completionPrefix());
    else {
        QTextCursor tc = textCursor();
        int extra = tempCompletion.length() - c->completionPrefix().length();
        tc.movePosition(QTextCursor::PreviousCharacter);
        tc.movePosition(QTextCursor::NextCharacter);
        tc.insertText(tempCompletion.right(extra));
        setTextCursor(tc);
    }
}
//! [4]

//! [5]
QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();

    int number = tc.columnNumber();
    tc.select(QTextCursor::LineUnderCursor);
    QString text = tc.selectedText();
    QString nextChar = text;
    if(text.size() > number)
        nextChar = nextChar.mid(number).left(1);
    else
        nextChar = "";
    QRegularExpression reg("[0-9a-zA-Z_|\\-|#|@|\\*|\\^|&|\\$|%|`|￥]");
    QRegularExpression revReg("[^0-9a-zA-Z_|\\-|#|@|\\*|\\^|&|\\$|%|`|￥]");
    if(number == text.length()) {
        QString currentChar = text;
        QString previousChar = text;
        previousChar = previousChar.right(1);
        if(!previousChar.contains(reg))
            return text;
        if(text.contains(revReg)) {
#if(QT_VERSION < QT_VERSION_CHECK(5,14,0))
            QStringList list = text.split(revReg, QString::SkipEmptyParts);
#else
            QStringList list = text.split(revReg, Qt::SkipEmptyParts);
#endif
            currentChar = list[list.size() - 1];
        }
        return currentChar;
    } else {
        if(nextChar.contains(reg))
            return text;
        else {
            QString previousStr = text;
            previousStr = previousStr.mid(0, number);
            QString previousChar = previousStr;
            previousChar = previousChar.right(1);
            if(!previousChar.contains(reg))
                return text;
            if(previousStr.contains(revReg)) {
#if(QT_VERSION < QT_VERSION_CHECK(5,14,0))
                QStringList list = previousStr.split(revReg, QString::SkipEmptyParts);
#else
                QStringList list = previousStr.split(revReg, Qt::SkipEmptyParts);
#endif
                previousStr = list[list.size() - 1];
            }
            return previousStr;
        }
    }
}
//! [5]

//! [6]
void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}
//! [6]

//! [7]
void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
//! [7]

//! [8]
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

//    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                      /*|| eow.contains(e->text().right(1))*/)) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
//! [8]

void CodeEditor::mousePressEvent(QMouseEvent *e)
{
    QPlainTextEdit::mousePressEvent(e);
}

void CodeEditor::mouseReleaseEvent(QMouseEvent *e)
{
    QTextCursor tc = textCursor();
    int lineNumber = tc.blockNumber();
    if(lineNumber != m_lineNumber) {
        emit pageClicked();
        m_lineNumber = lineNumber;
    }
    QPlainTextEdit::mousePressEvent(e);
}

void CodeEditor::keyReleaseEvent(QKeyEvent *e)
{
    if (!c || !c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right: {
            QTextCursor tc = textCursor();
            int lineNumber = tc.blockNumber();
            if(lineNumber != m_lineNumber) {
                emit pageClicked();
                m_lineNumber = lineNumber;
            }
            QPlainTextEdit::keyReleaseEvent(e);
            break;
        }
        default:
            break;
        }
    }
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint point = event->globalPos();
    emit pointChanged(point);
    QMenu *menu = createStandardContextMenu();
    menu->exec(point);
    event->accept();
}

void CodeEditor::onCursorPositionChanged()
{
    QTextCursor tc = textCursor();
    emit positonChanged(tc.blockNumber(), tc.positionInBlock(), tc.columnNumber(), tc.position(), blockCount());
}

void CodeEditor::replaceAll(QString text)
{
    QTextCursor tc = textCursor();
    tc.beginEditBlock();
    tc.select(QTextCursor::Document);
    tc.removeSelectedText();
    textCursor().insertText(text);
    tc.endEditBlock();
}

void CodeEditor::location(int row, int column)
{
    QTextCursor cursor(document());
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, row);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, column);
    setTextCursor(cursor);
    setFocus();
}

void CodeEditor::moveTextCursorEnd()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
    setFocus(Qt::MouseFocusReason);
}
