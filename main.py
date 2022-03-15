import os
import sys
from PySide6.QtCore import *
from PySide6.QtGui import *
from PySide6.QtWidgets import *

import image_utils

class Viewer(QLabel):
    def __init__(self, pixmap):
        super().__init__()
        
        self.pixmap = pixmap

        self.setSizePolicy(QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding))
        self.setAlignment(Qt.AlignCenter)

        self.setMinimumSize(1, 1)
        self.rescale()

    def updatePixmap(self, pixmap):
        self.pixmap = pixmap
        self.rescale()

    def resizeEvent(self, e: QResizeEvent) -> None:
        super().resizeEvent(e)
        self.rescale()

    def rescale(self):
        w = self.width()
        h = self.height()

        self.setPixmap(
            self.pixmap.scaled(
                w, h,
                Qt.KeepAspectRatio
            )
        )

class ColorBox(QLabel):
    def __init__(self, parent, color: QColor):
        super().__init__()

        self.parent = parent

        self.setFixedSize(75, 24)
        self.color = color
        self.update_text()

    def update_text(self):
        hex = self.color.name()
        self.setText(hex)

        style = f'''
        QLabel {{
            background-color: {hex};
            color: black;
            border-radius: 5px;
            font-family: monospce;
            font-weight: bold;
        }}
        
        '''

        self.setStyleSheet(style)
        self.setAlignment(Qt.AlignCenter)

    def mousePressEvent(self, event: QMouseEvent):
        self.color = QColorDialog.getColor(
            initial=self.color,
            options = QColorDialog.DontUseNativeDialog
        )
        self.update_text()
        self.parent.on_property_change()

class MainWin(QWidget):
    def __init__(self) -> None:
        super().__init__()
        # self.setGeometry(100, 100, 800, 600)

        self.icon_path = None

        self.setContentsMargins(0, 0, 0, 0)

        self.vbox = QVBoxLayout(self)

        self.image = QPixmap("resources/template.png")
        self.colorbox = ColorBox(self, QColor('#00ff00'))

        self.viewer = Viewer(self.image)
        self.vbox.addWidget(self.viewer)

        self.lower_hbox = QHBoxLayout()
        self.lower_hbox.setAlignment(Qt.AlignCenter)
        self.vbox.addLayout(self.lower_hbox)
        
        # left hbox
        left_vbox = QVBoxLayout()
        left_vbox.setAlignment(Qt.AlignTop)
        self.lower_hbox.addLayout(left_vbox)
        
        # color box
        left_vbox.addWidget(QLabel("Select Color"))
        left_vbox.addWidget(self.colorbox)
        
        # right hbox
        right_vbox = QVBoxLayout()
        right_vbox.setAlignment(Qt.AlignTop)
        self.lower_hbox.addLayout(right_vbox)

        # file dialog
        self.upload_button = QPushButton("Browse")
        self.upload_button.setFixedSize(75, 24)
        self.upload_button.clicked.connect(self.on_upload_click)

        # logo dialog
        right_vbox.addWidget(QLabel("Select Logo"))
        right_vbox.addWidget(self.upload_button)

        # save button
        self.lower_hbox.addStretch()
        self.lower_hbox.addWidget(QPushButton("Save"))

        self.update_image()

    def update_image(self):
        self.image = image_utils.create('template', self.colorbox.color)
        self.viewer.updatePixmap(self.image)

    def on_upload_click(self):
        path = QFileDialog.getOpenFileName(caption="Select Logo Image")[0]
        
        if not path:
            return

        if os.path.isfile(path):
            self.icon_path = path
            self.upload_button.setText(os.path.basename(path))

    def on_property_change(self):
        self.update_image()

if __name__ == '__main__':
    qapp = QApplication()
    qapp.setApplicationDisplayName("Awesome Car Creator")
    qapp.setStyle("Fusion")

    palette = QPalette()
    palette.setColor(QPalette.Window, QColor(53, 53, 53))
    palette.setColor(QPalette.WindowText, Qt.white)
    palette.setColor(QPalette.Base, QColor(25, 25, 25))
    palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
    palette.setColor(QPalette.ToolTipBase, Qt.black)
    palette.setColor(QPalette.ToolTipText, Qt.white)
    palette.setColor(QPalette.Text, Qt.white)
    palette.setColor(QPalette.Button, QColor(53, 53, 53))
    palette.setColor(QPalette.ButtonText, Qt.white)
    palette.setColor(QPalette.BrightText, Qt.red)
    palette.setColor(QPalette.Link, QColor(42, 130, 218))
    palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
    palette.setColor(QPalette.HighlightedText, Qt.black)
    qapp.setPalette(palette)

    win = MainWin()
    win.show()

    sys.exit(
        qapp.exec()
    )