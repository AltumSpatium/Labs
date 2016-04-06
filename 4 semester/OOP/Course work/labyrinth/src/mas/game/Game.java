//*************************************************************************************************

package mas.game;

//.................................................................................................

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.awt.image.BufferStrategy;
import javax.swing.JFrame;

//*************************************************************************************************

public class Game extends Canvas implements Runnable
{
	//.............................................................................................

	final static String frametitle = "Labyrinth";
	final static int framewidth = 960;
	final static int frameheight = 520;

	private final static long serialVersionUID = 1L;

	private boolean isrunning = false;
	private JFrame frame = new JFrame(frametitle);

	private Picture backimage = new Picture(Builder.typeStatic, null, "@logo.jpg");
	private Sound menusound = new Sound("@menu.wav");
	private Sound winnersound = new Sound("@winner.wav");
	private Sound losersound = new Sound("@loser");

	private Dialog mainmenu = new Dialog("@dialogs.map:mainmenu");
	private Dialog exitbox = new Dialog("@dialogs.map:exitbox");
	private Animate startwait = new Animate("@animate.map:loading");

	private Hero hero = new Hero(Builder.typeHero, "hero", "@objects.map:hero");

	//.............................................................................................

	public Game ()
	{
		this.setBackground(Color.BLACK);
		this.setPreferredSize(new Dimension(framewidth, frameheight));

		frame.setResizable(false);
		frame.setLayout(new BorderLayout());
		frame.add(this, BorderLayout.CENTER);
		frame.pack();

		Dimension screensize = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension dimension = frame.getPreferredSize();

		frame.setIconImage(Utils.loadImage("@icon.png"));
		frame.setLocation((screensize.width >> 1) - (dimension.width >> 1),
			(screensize.height >> 1) - (dimension.height >> 1));
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	//.............................................................................................

	public void run ()
	{
		long lasttime = System.currentTimeMillis();

		isrunning = true;
		requestFocus();
		startwait.runProcess(this, 1000, false);
		hero.loadProfile();

		while (isrunning)
		{
			update(System.currentTimeMillis() - lasttime);
			lasttime = System.currentTimeMillis();
			render();
		}
	}

	//.............................................................................................

	public void render ()
	{
		BufferStrategy bufferstrategy = getBufferStrategy();

		if (bufferstrategy == null)
		{
			createBufferStrategy(2);
			requestFocus();

			return;
		}

		Graphics graphics = bufferstrategy.getDrawGraphics();
		backimage.drawImage(graphics);
		graphics.dispose();
		bufferstrategy.show();
	}

	//.............................................................................................

	public void update (long _delta)
	{
		menusound.loopClip(0);

		Primitive contbutton = null;
		boolean cancontinue = ((hero.level > 1 || hero.levelmap != null) && hero.lives > 0 ? true : false);

		while ((contbutton = mainmenu.getPrimitive("continue", -1, contbutton)) != null)
			contbutton.setVisible(cancontinue);

		String command = mainmenu.runProcess(this, null, (cancontinue ? "continue" : null));

		menusound.stopClip();

		if (command == null)
		{
			isrunning = false;
		}
		else if (command.equals("exit"))
		{
			command = exitbox.runProcess(this, "no", "no");

			if (command.equals("yes"))
				isrunning = false;
		}
		else if (command.equals("new") || command.equals("continue"))
		{
			if (command.equals("new"))
				hero.clearState();

			startwait.runProcess(this, 1000, false);

			Level.Result result;
			Level level = new Level();

			while ((result = level.runProcess(hero, this)) == Level.Result.NEXT ||
				result == Level.Result.LOSER)
			{
				hero.saveProfile();

				if (result == Level.Result.LOSER)
				{
					if (--hero.lives <= 0)
						break;
				}

				startwait.runProcess(this, 1000, false);
			}

			if(result == Level.Result.CANCEL)
			{
			  hero.saveProfile();
			}			
			else if (result == Level.Result.WINNER)
			{
				winnersound.playClip();
				Animate winneranimate = new Animate("@animate.map:winner");
				Primitive label = winneranimate.getPrimitive("label", Builder.typeLabel, null);
				Primitive text = winneranimate.getPrimitive("text", Builder.typeLabel, null);

				text.setText("" + hero.mark);
				int labelwidth = label.getWidth(), textwidth = text.getWidth();
				text.addLeft((labelwidth - textwidth) >> 1);

				winneranimate.runProcess(this, 180000, true);
				winnersound.stopClip();
				hero.clearState();
				hero.saveProfile();
			}
			else if (result == Level.Result.LOSER)
			{
				losersound.loopClip(0);
				Animate loseranimate = new Animate("@animate.map:loser");
				Primitive label = loseranimate.getPrimitive("label", Builder.typeLabel, null);
				Primitive text = loseranimate.getPrimitive("text", Builder.typeLabel, null);

				text.setText("" + hero.mark);
				int labelwidth = label.getWidth(), textwidth = text.getWidth();
				text.addLeft((labelwidth - textwidth) >> 1);

				loseranimate.runProcess(this, 180000, true);
				losersound.stopClip();
				hero.clearState();
				hero.saveProfile();
			}
		}
	}

	//.............................................................................................

	public static void main (String[] args)
	{
		new Game().run();
		System.exit(0);
	}

	//.............................................................................................
}

//*************************************************************************************************
