package other_gui;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;

import javax.swing.Timer;

import animations.Fish;
import frames.MainGUI;
import game_logic.GameData;
import messages.BuzzInMessage;
import messages.NoBuzzMessage;
import messages.QuestionAnsweredMessage;
import messages.QuestionClickedMessage;
import messages.QuestionExpireMessage;
import messages.TimerMessage;
import server.Client;

//inherits from QuestionGUIElement
public class QuestionGUIElementNetworked extends QuestionGUIElement {

	private Client client;
	//very similar variables as in the AnsweringLogic class
	public Boolean hadSecondChance;
	private TeamGUIComponents currentTeam;
	private TeamGUIComponents originalTeam;
	//private Boolean questionDone;
	int teamIndex;
	int numTeams;
	//stores team index as the key to a Boolean indicating whether they have gotten a chance to answer the question
	private HashMap<Integer, Boolean> teamHasAnswered;
	
	private boolean buzzTime;
	private Fish f;
	
	// Timer
	protected Timer t;
	
	public QuestionGUIElementNetworked(String question, String answer, String category, int pointValue, int indexX, int indexY) {
		super(question, answer, category, pointValue, indexX, indexY);
		t = new Timer(1000, null);
		buzzTime = false;
		//Fish f = new Fish();
		//formatErrorPanel.add(f.getLabel());
		//f.setVisible(true);
	}
	
	//set the client and also set the map with the booleans to all have false
	public void setClient(Client client, int numTeams){
		this.client = client;
		this.numTeams = numTeams;
		teamIndex = client.getTeamIndex();
		teamHasAnswered = new HashMap<>();
		for (int i = 0; i< numTeams; i++) teamHasAnswered.put(i, false);
	}
	
	//returns whether every team has had a chance at answering this question
	public Boolean questionDone(){
		
		//if(this.questionDone == true)		// setQuestionDone was called as buzz time ran out
			//return true;
		Boolean questionDone = true;
		for (Boolean currentTeam : teamHasAnswered.values()) questionDone = questionDone && currentTeam;
		
		return questionDone;
	}
	
	public void setQuestionDone() {
		//this.questionDone = true;
		
	}
	
	//overrides the addActionListeners method in super class
	@Override
	public void addActionListeners(MainGUI mainGUI, GameData gameData){
		passButton.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				//send a buzz in message
				client.sendMessage(new BuzzInMessage(teamIndex));
				//stopTimer();
			}
			
		});
		
		gameBoardButton.addActionListener(new ActionListener(){
			
			@Override
			public void actionPerformed(ActionEvent e) {
				//send a question clicked message
				client.sendMessage((new TimerMessage(t)));
				client.sendMessage(new QuestionClickedMessage(getX(), getY()));
				
			}
		});
		
		submitAnswerButton.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				//send question answered message
				client.sendMessage(new QuestionAnsweredMessage(answerField.getText()));
				//stopTimer();
			}
			
		});
	}
	
	//override the resetQuestion method
	@Override
	public void resetQuestion(){
		super.resetQuestion();
		hadSecondChance = false;
		currentTeam = null;
		originalTeam = null;
		teamHasAnswered.clear();
		//reset teamHasAnswered map so all teams get a chance to anaswer again
		for (int i = 0; i< numTeams; i++) teamHasAnswered.put(i, false);
		
	}
	
	@Override
	public void populate(){
		super.populate();
		passButton.setText("Buzz In!");
	}
	
	public int getOriginalTeam(){
		return originalTeam.getTeamIndex();
	}

	public void updateAnnouncements(String announcement){
		announcementsLabel.setText(announcement);
	}
	
	public void setOriginalTeam(int team, GameData gameData){	// called from QuestionClickedAction
		originalTeam = gameData.getTeamDataList().get(team);
		
		t = new Timer(1000, null);
		updateTeam(team, gameData);
	}
	
	//update the current team of this question; set original team, or whoever is buzzing in from BuzzInMessage
	public void updateTeam(int team, GameData gameData){		
		currentTeam = gameData.getTeamDataList().get(team);
		passButton.setVisible(false);
		answerField.setText("");
		//if the current team is this client
		if (team == teamIndex){
			AppearanceSettings.setEnabled(true, submitAnswerButton, answerField);
			announcementsLabel.setText("Answer within 20 seconds!");
		}
		//if the current team is an opponent
		else{
			AppearanceSettings.setEnabled(false, submitAnswerButton, answerField);
			announcementsLabel.setText("It's "+currentTeam.getTeamName()+"'s turn to try to answer the question. "
					+ "Answer within 20 seconds!");
		}
		//mark down that this team has had a chance to answer
		teamHasAnswered.replace(team, true);
		hadSecondChance = false;
		teamLabel.setText(currentTeam.getTeamName());
		
		// Timer for new team
		startTimer();
	}
	
	//called from QuestionAnswerAction when there is an illformated answer
	public void illFormattedAnswer(){
		
		if (currentTeam.getTeamIndex() == teamIndex){
			announcementsLabel.setText("You had an illformatted answer. Please try again");
		}
		else{
			announcementsLabel.setText(currentTeam.getTeamName()+" had an illformatted answer. They get to answer again.");
		}
	}
	
	//set the gui to be a buzz in period, also called from QuestionAnswerAction
	public void setBuzzInPeriod(){
		
		passButton.setVisible(true);
		teamLabel.setText("");
		buzzTime = true;
		startTimer();
		
		if (teamHasAnswered.get(teamIndex)){
			AppearanceSettings.setEnabled(false, submitAnswerButton, answerField, passButton);
			announcementsLabel.setText("It's time to buzz in! But you've already had your chance..");
		}
		else{
			announcementsLabel.setText("Buzz in to answer the question!");
			passButton.setEnabled(true);
			AppearanceSettings.setEnabled(false, submitAnswerButton, answerField);
		}
		
	}
	
	
	public TeamGUIComponents getCurrentTeam(){
		return currentTeam;
	}
	
	public void startTimer() {
		stopTimer();
		
		//if (getCurrentTeam().getTeamIndex() == client.getTeamIndex()) {
			//f = new Fish();
			//f.setVisible(true);
			//updateAnnouncements("You have 20 seconds to answer!");
		//}
		//else
			//f.setVisible(false);
			

		t = new Timer(1000, new ActionListener() {
			int time = 21;	//21
			
			@Override
			public void actionPerformed(ActionEvent e) {
				this.time--;
				timerLabel.setText(":" + time);
				if (this.time == 0 && !buzzTime) {
					Timer timer = (Timer) e.getSource();
					timer.stop();
					//answerField.setText("");
					client.sendMessage(new QuestionExpireMessage()); // time expired
				}
				if (this.time == 0 && buzzTime){			// no one hit the buzzer
					Timer timer = (Timer) e.getSource();
					timer.stop();
					buzzTime = false;
					
	        		//if (getCurrentTeam().getTeamIndex() == client.getTeamIndex()) {
	        		client.sendMessage(new NoBuzzMessage(true));	
	        		//}
	        		//else {
	        			//client.sendMessage(new NoBuzzMessage(false));
	        		//}
				}
			}
		});
		t.start();	
	}
	
	
	public void stopTimer() {
		if(t.isRunning())
			t.stop();
		
		timerLabel.setText(":20");
	}
		
}
