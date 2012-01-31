package ui;

import java.awt.Component;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.AbstractList;
import java.util.ArrayList;


import views.EmbeddedView;
import model.CurvedTorus;
import model.Marker;
import model.Marker.MarkerType;
import model.OpenSquare;
import model.Surface;
import model.Vector;

public class SpaceGame {
	
	public static Surface S;
	
	public static Marker player;
	public static AbstractList<Marker> markers;
	
	public static EmbeddedView view;
	
	public static void main(String[] args) {
		initSurface();
		initMarkers();		
		initView();
		initControls();
	}	
	
	public static void initSurface(){
		S = new CurvedTorus();		
	}
	
	public static void initMarkers(){
		markers = new ArrayList<Marker>();
		player = new Marker(MarkerType.Rocket, S.makeCoordinates(50.0,50.0), new Vector(1.0,0.0));
		Marker sattelite = new Marker(MarkerType.Sattelite, S.makeCoordinates(2,3), new Vector(0.0,1.0));		
		markers.add( player );
		markers.add( sattelite );
	}
	
	public static void initView(){
		view = new EmbeddedView( S, player, markers );
	}
	
	public static void initControls(){
		Component comp = (Component) view.temp_v.getViewingComponent();
		
		comp.addKeyListener( new KeyAdapter() {
			public void keyPressed(KeyEvent e){								
				switch( e.getKeyCode() ){
				
				case KeyEvent.VK_RIGHT:
					player.getFacing().rotate( -0.3 );
					break;
					
				case KeyEvent.VK_LEFT:
					player.getFacing().rotate( 0.3 );
					break;
					
				case KeyEvent.VK_UP:
					player.move( 0.1 );
					break;
					
				case KeyEvent.VK_DOWN:
					player.move( -0.1 );
					break;				
								
				case KeyEvent.VK_V:
					view.toggleView();
					break;
					
				case KeyEvent.VK_F:
					view.moveCameraZ(10);
					break;
				
				case KeyEvent.VK_G:
					view.moveCameraZ(-10);
					break;
				}
				view.updateScene();
			}
		});		
	}	
}