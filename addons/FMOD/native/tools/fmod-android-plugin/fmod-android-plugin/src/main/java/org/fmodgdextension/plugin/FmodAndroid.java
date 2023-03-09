package org.fmodgdextension.plugin;

import android.app.Activity;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import org.godotengine.godot.Godot;
import org.fmod.FMOD;

public class FmodAndroid extends org.godotengine.godot.plugin.GodotPlugin {

    public FmodAndroid(Godot godot) {
        super(godot);
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FmodAndroidPlugin";
    }

    @Nullable
    @Override
    public View onMainCreate(Activity activity) {
        FMOD.init(activity);
        return super.onMainCreate(activity);
    }

    @Override
    public void onMainDestroy() {
        FMOD.close();
    }

    static {
        System.loadLibrary("fmodL");
        System.loadLibrary("fmodstudioL");
    }
}
