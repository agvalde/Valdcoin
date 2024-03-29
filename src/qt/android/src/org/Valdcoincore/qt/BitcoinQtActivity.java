package org.valdcoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class ValdcoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File valdcoinDir = new File(getFilesDir().getAbsolutePath() + "/.valdcoin");
        if (!valdcoinDir.exists()) {
            valdcoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
