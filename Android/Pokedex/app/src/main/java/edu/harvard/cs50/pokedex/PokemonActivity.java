package edu.harvard.cs50.pokedex;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class PokemonActivity extends AppCompatActivity {
    private TextView nameTextView;
    private TextView numberTextView;
    private TextView type1TextView;
    private TextView type2TextView;
    private String url;
    private RequestQueue requestQueue;

    private boolean isPressed;
    private TextView catchButton;
    private SharedPreferences sharedPreference;
    private SharedPreferences.Editor editor;
    //private Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pokemon);

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        url = getIntent().getStringExtra("url");
        nameTextView = findViewById(R.id.pokemon_name);
        numberTextView = findViewById(R.id.pokemon_number);
        type1TextView = findViewById(R.id.pokemon_type1);
        type2TextView = findViewById(R.id.pokemon_type2);

        catchButton = findViewById(R.id.catch_release);
        sharedPreference = getApplicationContext().getSharedPreferences("Pokecatcher", 0);
        editor = sharedPreference.edit();
        isPressed = false;

        load();
    }

    public void load() {
        type1TextView.setText("");
        type2TextView.setText("");

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    nameTextView.setText(response.getString("name"));
                    numberTextView.setText(String.format("#%03d", response.getInt("id")));

                    JSONArray typeEntries = response.getJSONArray("types");
                    for (int i = 0; i < typeEntries.length(); i++) {
                        JSONObject typeEntry = typeEntries.getJSONObject(i);
                        int slot = typeEntry.getInt("slot");
                        String type = typeEntry.getJSONObject("type").getString("name");

                        if (slot == 1) {
                            type1TextView.setText(type);
                        }
                        else if (slot == 2) {
                            type2TextView.setText(type);
                        }
                    }

                    if (sharedPreference.getBoolean(response.getString("name"), false)) {
                        catchButton.setText("Catch");
                    }
                    else {
                        catchButton.setText("Release");
                    }

                } catch (JSONException e) {
                    Log.e("cs50", "Pokemon json error", e);
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("cs50", "Pokemon details error", error);
            }
        });

        requestQueue.add(request);
    }

    public void toggleCatch(View view) {
        //Button isPressed = (Button) view;
        //Pokemon pokemonName = new Pokemon();
        if (isPressed) {
            editor.putBoolean(nameTextView.getText().toString(), false).apply();
            catchButton.setText("Catch");
            //isPressed = false;
        }
        if (!isPressed) {
            editor.putBoolean(nameTextView.getText().toString(), true).apply();
            catchButton.setText("Release");
        }
        isPressed = true;

        //if (!getPreferences(Context.MODE_PRIVATE).contains(pokemonName)) {
          //  getPreferences(Context.MODE_PRIVATE).edit().putBoolean(pokemonName, true).apply();
        //}
        //else {
          //  getPreferences(Context.MODE_PRIVATE).edit().putBoolean(pokemonName, false).apply();
        //}
        //if (getPreferences(Context.MODE_PRIVATE).getBoolean(pokemonName, true)) {
          //  isPressed.setText(("Release"));
        //}
        //else if (getPreferences(Context.MODE_PRIVATE).getBoolean(pokemonName, false)) {
          //  getPreferences(Context.MODE_PRIVATE).edit().remove(pokemonName).apply();
            //isPressed.setText(("Catch!"));
        //}
    }
}
