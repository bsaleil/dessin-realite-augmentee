#include "gesturewidget.hpp"

GestureWidget::GestureWidget(bool *f, QPen *p)
{
	// On récupère les pointeurs sur l'état du tableau
	pen = p;
	firstPoint = f;
	
	// Création de l'interface
	QHBoxLayout *layout = new QHBoxLayout();
	
	// Boutons d'indication
	buttonColor = new GestureButton("button");
		buttonColor->setStyleSheet("background-color: #000000");
	labelWidth = new GestureButton("5","5");
		labelWidth->setStyleSheet("border: none");
		
	
	// Boutons du pinceau
	buttonRed = new GestureButton("red");
	buttonGreen = new GestureButton("green");
	buttonBlue = new GestureButton("blue");
	buttonBlack = new GestureButton("black");
	buttonEraser = new GestureButton("eraser");
	buttonMinus = new GestureButton("-","-");
	buttonPlus = new GestureButton("+","+");
	
	// Ajout des styles
	buttonRed->setStyleSheet("background-color: #ff0000");
	buttonGreen->setStyleSheet("background-color: #00ff00");
	buttonBlue->setStyleSheet("background-color: #0000ff");
	buttonBlack->setStyleSheet("background-color: #000000");
	QPixmap pixmap("eraser.png"); QIcon icon(pixmap); buttonEraser->setIcon(icon);
	
	// Construction du layout
	layout->addWidget(buttonColor);
	layout->addWidget(labelWidth);
	layout->addWidget(buttonRed);
	layout->addWidget(buttonGreen);
	layout->addWidget(buttonBlue);
	layout->addWidget(buttonBlack);
	layout->addWidget(buttonEraser);
	layout->addWidget(buttonMinus);
	layout->addWidget(buttonPlus);	
	
	setLayout(layout);
}

void GestureWidget::mousePosition(int x, int y)
{
	// On récupère le widget sous la position du curseur
	GestureButton *widget = (GestureButton*)childAt(x,y);
	
	// Si le widget est valide et que le pinceau n'est pas en mode dessin
	if (widget && *firstPoint)
	{
		// On récupère l'id du bouton cliqué
		QString id = widget->click();
		
		// Si rouge, on change le pinceau, et on change de couleur le bouton
		// Idem pour les autres couleurs
		if (id == "red")
		{
			pen->setColor(QColor("#ff0000"));
			buttonColor->setStyleSheet("background-color: #ff0000");
		}
		else if (id == "green")
		{
			pen->setColor(QColor("#00ff00"));
			buttonColor->setStyleSheet("background-color: #00ff00");
		}
		else if (id == "blue")
		{
			pen->setColor(QColor("#0000ff"));
			buttonColor->setStyleSheet("background-color: #0000ff");
		}
		else if (id == "black")
		{
			pen->setColor(QColor("#000000"));
			buttonColor->setStyleSheet("background-color: #000000");
		}
		else if (id == "eraser")
		{
			pen->setColor(QColor("#ffffff"));
			buttonColor->setStyleSheet("background-color: #ffffff");
		}
		// Si bouton "-" et que l'épaisseur > 1, on change l'épaisseur et MAJ du label
		// Idem pour "+"
		else if (id == "-" && pen->width() > 1)
		{
			pen->setWidth(pen->width() - 1);
			labelWidth->setText(QString::number(pen->width()));
		}
		else if (id == "+")
		{
			if (pen->width() < 20)
			{
				pen->setWidth(pen->width() + 1);
				labelWidth->setText(QString::number(pen->width()));
			}
		}
			
	}
}

void GestureWidget::changeButtonColor(QColor c)
{
	buttonColor->setStyleSheet("background-color : " + c.name());
}
