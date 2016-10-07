package smart.endlessnews;

import android.content.Context;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;

class OnScaleTouchListener implements View.OnTouchListener{
    private final ScaleGestureDetector scaleGestureDetector;

    OnScaleTouchListener(Context ctx) {
        scaleGestureDetector = new ScaleGestureDetector(ctx, new ScaleGestureListener());
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        return scaleGestureDetector.onTouchEvent(event);
    }

    private final class ScaleGestureListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        public boolean onScale(ScaleGestureDetector detector) {
            resize(detector);
            return true;
        }
    }

    void resize(ScaleGestureDetector detector) {
    }
}
