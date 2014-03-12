/*
 * File: ProgramHierarchy.java
 * Name:
 * Section Leader:
 * ---------------------------
 * This file is the starter file for the ProgramHierarchy problem.
 */

import acm.graphics.*;
import acm.program.*;
import java.awt.*;

/* This program makes a centered Program Hierarchy graphic of boxes with text centered in them.
 * The boxes for a tree with one box at the top and three at the base.
 */

public class ProgramHierarchy extends GraphicsProgram {
    public void run() {
        double program_y = (getHeight() - 3 * BOX_HEIGHT) / 2.0;
        double program_x = (getWidth() - BOX_WIDTH) / 2.0;
        makeBoxedLabel(program_x, program_y, "Program");
        double graphics_x = (getWidth() - (3 * BOX_WIDTH + 2 * CONNECTOR)) / 2.0;
        double lower_y = program_y + BOX_HEIGHT * 2;
        makeBoxedLabel(graphics_x, lower_y, "GraphicsProgram");
        makeBoxedLabel(program_x, lower_y, "ConsoleProgram");
        double dialog_x = program_x + BOX_WIDTH + CONNECTOR;
        makeBoxedLabel(dialog_x, lower_y, "DialogProgram");
        drawLine(program_x, program_y, graphics_x, lower_y);
        drawLine(program_x, program_y, program_x, lower_y);
        drawLine(program_x, program_y, dialog_x, lower_y);
    }

    /* Makes a box starting at (x,y) with a label centered inside.
     */

    private void makeBoxedLabel(double x, double y, String text) {
        GRect box = new GRect(x, y, BOX_WIDTH, BOX_HEIGHT);
        GLabel words = new GLabel (text);
        double dx = (BOX_WIDTH - words.getWidth()) / 2.0;
        double dy = (BOX_HEIGHT / 2.0) + (words.getAscent() / 2.0);
        GLabel label = new GLabel (text, x + dx, y + dy);
        add(box);
        add(label);
    }

    /* Creates a line from the center of the base of a rectangle that starts at (x1, y1)
     * to the center of the top of a rectangle that starts at (x2, y2).
     */

    private void drawLine(double x1, double y1, double x2, double y2) {
        double a1 = x1 + BOX_WIDTH / 2.0;
        double b1 = y1 + BOX_HEIGHT;
        double a2 = x2 + BOX_WIDTH / 2.0;
        double b2 = y2;
        GLine line = new GLine(a1, b1, a2, b2);
        add(line);
    }

    /* Private constants */
    private static final double BOX_HEIGHT = 50;
    private static final double BOX_WIDTH = 120;
    private static final double CONNECTOR = 15;
}
