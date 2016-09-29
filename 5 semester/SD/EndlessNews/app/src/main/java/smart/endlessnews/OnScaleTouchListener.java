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

    private final class ScaleGestureListener implements ScaleGestureDetector.OnScaleGestureListener {
        public boolean onScale(ScaleGestureDetector detector) {
            try {
                if (detector.getScaleFactor() > 1) scaleDilute(detector.getScaleFactor());
                else if (detector.getScaleFactor() < 1) scalePinch(detector.getScaleFactor());
                return true;
            }
            catch (Exception e) {
                e.printStackTrace();
            }
            return false;
        }

        public boolean onScaleBegin(ScaleGestureDetector detector) {
            return true;
        }

        public void onScaleEnd(ScaleGestureDetector detector) {
        }
    }

    void scaleDilute(float coeff) {
    }

    void scalePinch(float coeff) {
    }
}
