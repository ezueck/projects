package com.example.ezueckg.firstapplication;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    //Declare variable instances of our XML elements
    private TextView question;
    private Button answer1;
    private Button answer2;
    private Button answer3;
    private Button answer4;
    private String[] answerChoices;
    private String[] questions;
    private String[] correctAnswer;
    int questionNum;
    int answerNum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        question = (TextView) findViewById(R.id.question);
        answer1 = (Button) findViewById(R.id.answer1);
        answer2 = (Button) findViewById(R.id.answer2);
        answer3 = (Button) findViewById(R.id.answer3);
        answer4 = (Button) findViewById(R.id.answer4);
        answerChoices = getResources().getStringArray(R.array.answers);
        questions = getResources().getStringArray(R.array.questions);
        correctAnswer = getResources().getStringArray(R.array.correct_answers);

        questionNum = 0;
        answerNum = 0;
    }

    private void setUpQuestion() {
        question.setText(questions[questionNum]);
        answer1.setText(answerChoices[answerNum++] );
        answer2.setText(answerChoices[answerNum++] );
        answer3.setText(answerChoices[answerNum++] );
        answer4.setText(answerChoices[answerNum++] );
    }

    private void answerClicked(View v){
        Button clicked = (Button) v;
        if (clicked.getText().equals(correctAnswer[questionNum++])){
            Toast.makeText(this, "Correct", Toast.LENGTH_SHORT).show();
        }
        else {
            Toast.makeText(this, "Incorrect", Toast.LENGTH_SHORT).show();
        }

        setUpQuestion();
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
