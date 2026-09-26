#include "PluginEditor.h"
namespace {
const juce::Colour black(0xff070b11), panel(0xff101722), panelRaised(0xff151d29), line(0xff283344),
                   text(0xfff2f4f8), muted(0xff7f8999), cyan(0xffffad45), violet(0xffa85df5);

void label(juce::Label& l,float size,juce::Colour colour=muted) {
    l.setFont(juce::Font(juce::FontOptions(size)));l.setColour(juce::Label::textColourId,colour);
}

void glowRounded(juce::Graphics& g, juce::Rectangle<float> r, float radius, juce::Colour colour, float strength=.16f)
{
    // Cheap bloom made from layered translucent shapes. It stays lightweight in
    // JUCE while giving selected cards/controls a soft neon halo.
    for(int i=5;i>=1;--i) {
        const float spread=float(i)*2.2f;
        g.setColour(colour.withAlpha(strength*(6.f-float(i))/32.f));
        g.fillRoundedRectangle(r.expanded(spread),radius+spread);
    }
}

void panelCard(juce::Graphics& g, juce::Rectangle<float> r, juce::Colour accent, bool selected=false)
{
    if(selected) glowRounded(g,r,12.f,accent,.22f);
    g.setGradientFill(juce::ColourGradient(panelRaised.withAlpha(.96f),r.getTopLeft(),panel.darker(.18f),r.getBottomRight(),false));
    g.fillRoundedRectangle(r,12.f);
    g.setColour(juce::Colours::white.withAlpha(.025f));
    g.drawRoundedRectangle(r.reduced(.75f),11.3f,1.f);
    g.setColour(accent.withAlpha(selected?.82f:.30f));
    g.drawRoundedRectangle(r,12.f,1.1f);
}
}
RefMatchLookAndFeel::RefMatchLookAndFeel()
{
    setColour(juce::TextButton::buttonColourId,panel);
    setColour(juce::TextButton::textColourOffId,text);
    setColour(juce::TextButton::textColourOnId,black);
    setColour(juce::TextButton::buttonOnColourId,cyan);
    setColour(juce::Slider::thumbColourId,cyan);
    setColour(juce::Slider::trackColourId,cyan.withAlpha(.5f));
    setColour(juce::Slider::backgroundColourId,line);
    setColour(juce::Slider::textBoxTextColourId,text);
    setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);
    setColour(juce::ComboBox::backgroundColourId,panel);
    setColour(juce::ComboBox::textColourId,text);
    setColour(juce::ComboBox::outlineColourId,line);
    setColour(juce::ComboBox::arrowColourId,violet);
    setColour(juce::PopupMenu::backgroundColourId,panel);
    setColour(juce::PopupMenu::textColourId,text);
    setColour(juce::ToggleButton::textColourId,text);
    setColour(juce::ToggleButton::tickColourId,cyan);
    setColour(juce::TextEditor::backgroundColourId,panel);
    setColour(juce::TextEditor::textColourId,text);
    setColour(juce::TextEditor::outlineColourId,line);
    setColour(juce::TextEditor::focusedOutlineColourId,cyan);
}

void RefMatchLookAndFeel::drawComboBox(juce::Graphics& g,int width,int height,bool isButtonDown,int,int,int,int,juce::ComboBox& box)
{
    auto r=juce::Rectangle<float>(0.f,0.f,float(width),float(height)).reduced(.75f);
    const bool over=box.isMouseOverOrDragging();
    const auto fill=panelRaised.withMultipliedBrightness(isButtonDown?.94f:over?1.07f:1.f);

    if(over) glowRounded(g,r,6.f,violet,.055f);
    g.setGradientFill(juce::ColourGradient(fill.brighter(.025f),r.getTopLeft(),fill.darker(.07f),r.getBottomRight(),false));
    g.fillRoundedRectangle(r,6.f);
    g.setColour(line.brighter(over?.10f:.03f).withAlpha(over?.88f:.72f));
    g.drawRoundedRectangle(r,6.f,1.f);

    // Small clean chevron instead of JUCE's default oversized arrow area.
    const float cx=r.getRight()-13.f, cy=r.getCentreY();
    juce::Path chevron;
    chevron.startNewSubPath(cx-4.f,cy-2.f);
    chevron.lineTo(cx,cy+2.f);
    chevron.lineTo(cx+4.f,cy-2.f);
    g.setColour(violet.withAlpha(over?1.f:.90f));
    g.strokePath(chevron,juce::PathStrokeType(1.7f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
}

juce::Font RefMatchLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return juce::Font(juce::FontOptions(10.6f,juce::Font::bold));
}

void RefMatchLookAndFeel::drawButtonBackground(juce::Graphics& g,juce::Button& button,const juce::Colour& colour,bool over,bool down)
{
    auto r=button.getLocalBounds().toFloat().reduced(.75f);
    const bool on=button.getToggleState();
    const bool activeWhenOff=bool(button.getProperties()["activeWhenOff"]);
    const bool visualOn=activeWhenOff?!on:on;
    const bool compact=bool(button.getProperties()["compactType"]);
    const bool dual=bool(button.getProperties()["dualAccent"]);
    const bool glow=bool(button.getProperties()["glow"]);
    const bool roundSwitch=bool(button.getProperties()["roundSwitch"]);

    if(roundSwitch) {
        auto circle=r.withSizeKeepingCentre(std::min(r.getWidth(),r.getHeight()),std::min(r.getWidth(),r.getHeight()));
        glowRounded(g,circle, circle.getWidth()*.5f, violet, over?.18f:.11f);
        g.setGradientFill(juce::ColourGradient(panelRaised.brighter(over?.10f:.04f),circle.getTopLeft(),black.brighter(.06f),circle.getBottomRight(),false));
        g.fillEllipse(circle);
        g.setColour(juce::Colours::white.withAlpha(.035f));g.drawEllipse(circle.reduced(1.f),1.f);
        g.setColour(line.brighter(.12f).withAlpha(.95f));g.drawEllipse(circle,1.1f);
        const auto icon=text.withAlpha(down?.65f:over?1.f:.92f);
        g.setColour(icon);
        const float cx=circle.getCentreX(), cy=circle.getCentreY();
        juce::Path top,bottom;
        top.startNewSubPath(cx-10.f,cy-5.f);top.lineTo(cx+8.f,cy-5.f);top.lineTo(cx+4.f,cy-9.f);
        bottom.startNewSubPath(cx+10.f,cy+5.f);bottom.lineTo(cx-8.f,cy+5.f);bottom.lineTo(cx-4.f,cy+9.f);
        g.strokePath(top,juce::PathStrokeType(1.8f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        g.strokePath(bottom,juce::PathStrokeType(1.8f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        return;
    }

    if(button.getProperties().contains("captureProgress")) {
        const float progress=juce::jlimit(0.0f,1.0f,float(button.getProperties()["captureProgress"]));
        const auto accent=button.findColour(juce::TextButton::buttonOnColourId);
        const bool ready=progress>=.999f;
        const auto fill=panelRaised.withMultipliedBrightness(down?.92f:over?1.06f:1.f);

        // Neutral base before capture and behind the in-button progress fill.
        g.setGradientFill(juce::ColourGradient(fill.brighter(.025f),r.getTopLeft(),fill.darker(.085f),r.getBottomRight(),false));
        g.fillRoundedRectangle(r,8.f);

        if(ready) {
            // READY has two visual states:
            // - while capture is still running, keep the existing active/live appearance;
            // - after the user stops capture, settle into a calmer completed appearance.
            if(on) {
                glowRounded(g,r,8.f,accent,.18f);
                g.setGradientFill(juce::ColourGradient(accent.brighter(.10f).withAlpha(.88f),r.getTopLeft(),
                                                       accent.darker(.28f).withAlpha(.58f),r.getTopRight(),false));
            } else {
                glowRounded(g,r,8.f,accent,.045f);
                g.setGradientFill(juce::ColourGradient(accent.brighter(.035f).withAlpha(.54f),r.getTopLeft(),
                                                       accent.darker(.34f).withAlpha(.34f),r.getTopRight(),false));
            }
            g.fillRoundedRectangle(r,8.f);
        } else if(progress>0.001f) {
            // 0-8 s: integrated left-to-right progress inside the capture button.
            juce::Graphics::ScopedSaveState save(g);
            g.reduceClipRegion(r.withWidth(r.getWidth()*progress).getSmallestIntegerContainer());
            g.setGradientFill(juce::ColourGradient(accent.brighter(.05f).withAlpha(.90f),r.getTopLeft(),
                                                   accent.darker(.22f).withAlpha(.66f),r.getTopRight(),false));
            g.fillRoundedRectangle(r,8.f);
        }

        g.setColour((ready?accent:line).withAlpha(ready?(on?.92f:.68f):.74f));
        g.drawRoundedRectangle(r,8.f,ready?(on?1.25f:1.05f):1.f);
        g.setColour(juce::Colours::white.withAlpha(ready?(on?.055f:.025f):.025f));
        g.drawRoundedRectangle(r.reduced(1.f),7.f,.8f);
        return;
    }

    if(bool(button.getProperties()["autoGainPill"])) {
        const auto accent=cyan;
        const bool measuring=bool(button.getProperties()["autoGainMeasuring"]);
        const float progress=juce::jlimit(0.0f,1.0f,float(button.getProperties()["autoGainProgress"]));
        if(over || on || measuring) glowRounded(g,r,10.f,accent,measuring?.10f:(on?.15f:.07f));
        const auto fill=panelRaised.withMultipliedBrightness(down?.92f:over?1.06f:1.f);
        g.setGradientFill(juce::ColourGradient(fill.brighter(.025f),r.getTopLeft(),fill.darker(.085f),r.getBottomRight(),false));
        g.fillRoundedRectangle(r,10.f);

        // During Auto Gain measurement, use the pill itself as the progress indicator.
        // This mirrors the RECORD MIX / RECORD REF interaction and avoids a cramped timer.
        if(measuring && progress>0.001f) {
            juce::Graphics::ScopedSaveState save(g);
            g.reduceClipRegion(r.withWidth(r.getWidth()*progress).getSmallestIntegerContainer());
            g.setGradientFill(juce::ColourGradient(accent.brighter(.08f).withAlpha(.78f),r.getTopLeft(),
                                                   accent.darker(.26f).withAlpha(.48f),r.getTopRight(),false));
            g.fillRoundedRectangle(r,10.f);
        }

        g.setColour(((on || measuring)?accent:line.brighter(.04f)).withAlpha((on || measuring)?.95f:.88f));
        g.drawRoundedRectangle(r,10.f,(on || measuring)?1.35f:1.1f);
        g.setColour(juce::Colours::white.withAlpha(.025f));
        g.drawRoundedRectangle(r.reduced(1.f),9.f,.8f);
        return;
    }

    if(bool(button.getProperties()["loopGroupButton"])) {
        // LOOP is visually grouped with its ON/OFF toggle. The parent editor
        // paints the shared container; this left segment only adds the active
        // loop-menu highlight when the editor is open.
        if(on) {
            glowRounded(g,r,9.f,violet,.13f);
            g.setGradientFill(juce::ColourGradient(violet.withAlpha(.20f),r.getTopLeft(),
                                                   panelRaised.withAlpha(.96f),r.getBottomRight(),false));
            g.fillRoundedRectangle(r,9.f);
            g.setColour(violet.withAlpha(.92f));
            g.drawRoundedRectangle(r,9.f,1.25f);
        }
        return;
    }

    const bool softAction=bool(button.getProperties()["softAction"]);
    if((glow && visualOn) || visualOn) glowRounded(g,r,compact?6.f:8.f,dual?violet:colour,compact?.20f:.17f);

    if(softAction) {
        if(over || visualOn) glowRounded(g,r,9.f,visualOn?(dual?violet:colour):line,.08f);
        const auto fill=panelRaised.withMultipliedBrightness(down?.90f:over?1.08f:1.f);
        g.setGradientFill(juce::ColourGradient(fill.brighter(.035f),r.getTopLeft(),fill.darker(.075f),r.getBottomRight(),false));
        g.fillRoundedRectangle(r,9.f);
        g.setColour((visualOn?(dual?violet:colour):line).withAlpha(visualOn?.86f:.70f));
        g.drawRoundedRectangle(r,9.f,visualOn?1.25f:1.f);
        g.setColour(juce::Colours::white.withAlpha(over?.05f:.02f));g.drawRoundedRectangle(r.reduced(1.f),8.f,.8f);
        return;
    }

    if(compact) {
        const auto fill=panelRaised.withMultipliedBrightness(down?.88f:over?1.10f:1.f);
        g.setColour(fill);g.fillRoundedRectangle(r,6.f);
        g.setColour((visualOn?colour:line).withAlpha(visualOn?.95f:.62f));g.drawRoundedRectangle(r,6.f,visualOn?1.35f:1.f);
        return;
    }

    if((visualOn || bool(button.getProperties()["forceDual"])) && dual)
        g.setGradientFill(juce::ColourGradient(cyan,r.getTopLeft(),violet,r.getTopRight(),false));
    else if(visualOn)
        g.setGradientFill(juce::ColourGradient(colour.brighter(.08f),r.getTopLeft(),colour.darker(.08f),r.getBottomRight(),false));
    else {
        const auto fill=panelRaised.withMultipliedBrightness(down?.88f:over?1.10f:1.f);
        g.setGradientFill(juce::ColourGradient(fill.brighter(.04f),r.getTopLeft(),fill.darker(.08f),r.getBottomRight(),false));
    }
    g.fillRoundedRectangle(r,8.f);
    g.setColour((visualOn?colour:line).withAlpha(visualOn?.90f:.72f));g.drawRoundedRectangle(r,8.f,visualOn?1.2f:1.f);
    g.setColour(juce::Colours::white.withAlpha(over?.055f:.025f));g.drawRoundedRectangle(r.reduced(1.f),7.f,.8f);
}
void RefMatchLookAndFeel::drawButtonText(juce::Graphics& g,juce::TextButton& button,bool over,bool down)
{
    if(bool(button.getProperties()["autoGainPill"])) {
        const auto r=button.getLocalBounds().toFloat();
        const auto accent=cyan;
        const auto raw=button.getButtonText();
        const int sep=raw.indexOfChar('|');
        const auto left=sep>=0?raw.substring(0,sep):raw;
        const auto right=sep>=0?raw.substring(sep+1):juce::String();
        const float iconX=r.getX()+12.f, cy=r.getCentreY();
        g.setColour(accent.withAlpha(button.isEnabled()?.98f:.62f));
        const float heights[3]={8.f,15.f,11.f};
        for(int i=0;i<3;++i) {
            const float x=iconX-7.f+i*5.f;
            g.fillRoundedRectangle(x,cy-heights[i]*.5f,2.4f,heights[i],1.2f);
        }
        g.setColour(text.withAlpha(button.isEnabled()?(down?.72f:over?1.f:.94f):.56f));
        g.setFont(juce::Font(juce::FontOptions(9.8f,juce::Font::bold)));
        const float dividerX = right.isNotEmpty() ? r.getRight()-50.f : r.getRight();
        g.drawText(left,juce::Rectangle<float>(iconX+10.f,r.getY(),juce::jmax(28.f,dividerX-(iconX+12.f)),r.getHeight()),juce::Justification::centredLeft);
        if(right.isNotEmpty()) {
            g.setColour(line.brighter(.10f).withAlpha(.82f));
            g.fillRoundedRectangle(dividerX,r.getY()+6.f,1.2f,r.getHeight()-12.f,.6f);
            g.setColour(text.withAlpha(button.isEnabled()?.72f:.46f));
            g.setFont(juce::Font(juce::FontOptions(9.2f)));
            g.drawText(right,juce::Rectangle<float>(dividerX+4.f,r.getY(),r.getRight()-dividerX-12.f,r.getHeight()),juce::Justification::centredRight);
        }
        return;
    }
    if(bool(button.getProperties()["trashIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const auto c=text.withAlpha(down?.68f:over?1.f:.88f);
        const float x=r.getX()+15.f,cy=r.getCentreY();
        g.setColour(c);
        g.drawRoundedRectangle(x-5.f,cy-5.f,10.f,12.f,1.5f,1.2f);
        g.drawLine(x-7.f,cy-7.f,x+7.f,cy-7.f,1.2f);
        g.drawLine(x-3.f,cy-9.f,x+3.f,cy-9.f,1.2f);
        g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));
        g.drawText(button.getButtonText(),juce::Rectangle<float>(x+12.f,r.getY(),r.getRight()-(x+15.f),r.getHeight()),juce::Justification::centredLeft);
        return;
    }
    if(bool(button.getProperties()["resetIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const auto c=text.withAlpha(down?.68f:over?1.f:.90f);
        g.setColour(c);

        // Use the actual Unicode clockwise open-circle arrow requested for Reset.
        // This avoids a hand-drawn approximation and keeps the icon visually clean.
        g.setFont(juce::Font(juce::FontOptions(21.f)));
        g.drawText(juce::String::fromUTF8("\xE2\x9F\xB3"),
                   juce::Rectangle<float>(r.getX()+7.f,r.getY(),30.f,r.getHeight()),
                   juce::Justification::centred);

        g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));
        g.drawText(button.getButtonText(),
                   juce::Rectangle<float>(r.getX()+38.f,r.getY(),r.getWidth()-43.f,r.getHeight()),
                   juce::Justification::centredLeft);
        return;
    }
    if(bool(button.getProperties()["playerIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const auto c=text.withAlpha(down?.70f:over?1.f:.94f);
        const bool paused=button.getToggleState();
        g.setColour(c);
        if(paused) {
            const float w=3.0f,h=12.0f,gap=3.5f;
            const float cx=r.getCentreX(),cy=r.getCentreY();
            g.fillRoundedRectangle(cx-gap-w,cy-h*.5f,w,h,1.1f);
            g.fillRoundedRectangle(cx+gap,cy-h*.5f,w,h,1.1f);
        } else {
            const float cx=r.getCentreX()+1.f,cy=r.getCentreY();
            juce::Path tri;
            tri.startNewSubPath(cx-5.f,cy-7.f);
            tri.lineTo(cx+7.f,cy);
            tri.lineTo(cx-5.f,cy+7.f);
            tri.closeSubPath();
            g.fillPath(tri);
        }
        return;
    }
    if(bool(button.getProperties()["loopIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const bool menuOpen=button.getToggleState();
        const auto c=(menuOpen?violet:text).withAlpha(down?.70f:over?1.f:.92f);
        const float cx=r.getX()+19.f, cy=r.getCentreY();
        g.setColour(c);
        // Compact two-arrow loop glyph, drawn as vectors so it scales cleanly.
        juce::Path top,bottom;
        top.startNewSubPath(cx-7.f,cy-3.f);
        top.cubicTo(cx-6.f,cy-9.f,cx+4.f,cy-10.f,cx+8.f,cy-5.f);
        top.lineTo(cx+8.f,cy-8.f);
        top.lineTo(cx+12.f,cy-4.f);
        top.lineTo(cx+7.f,cy-1.f);
        bottom.startNewSubPath(cx+7.f,cy+3.f);
        bottom.cubicTo(cx+6.f,cy+9.f,cx-4.f,cy+10.f,cx-8.f,cy+5.f);
        bottom.lineTo(cx-8.f,cy+8.f);
        bottom.lineTo(cx-12.f,cy+4.f);
        bottom.lineTo(cx-7.f,cy+1.f);
        g.strokePath(top,juce::PathStrokeType(1.9f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        g.strokePath(bottom,juce::PathStrokeType(1.9f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));
        g.setColour(text.withAlpha(down?.70f:over?1.f:.94f));
        g.drawText(button.getButtonText(),
                   juce::Rectangle<float>(r.getX()+38.f,r.getY(),r.getWidth()-42.f,r.getHeight()),
                   juce::Justification::centredLeft);
        return;
    }
    if(bool(button.getProperties()["closeLoopIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const auto c=text.withAlpha(down?.68f:over?1.f:.94f);
        const float cx=r.getX()+18.f, cy=r.getCentreY();
        g.setColour(c);
        g.drawLine(cx-5.f,cy-5.f,cx+5.f,cy+5.f,1.6f);
        g.drawLine(cx+5.f,cy-5.f,cx-5.f,cy+5.f,1.6f);
        g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));
        g.drawText(button.getButtonText(),juce::Rectangle<float>(cx+13.f,r.getY(),r.getRight()-(cx+16.f),r.getHeight()),juce::Justification::centredLeft);
        return;
    }
    if(bool(button.getProperties()["recordIcon"])) {
        const auto r=button.getLocalBounds().toFloat();
        const bool on=button.getToggleState();
        const auto accent=button.findColour(juce::TextButton::buttonOnColourId);
        const float iconX=r.getX()+18.f, cy=r.getCentreY();
        if(on) glowRounded(g,{iconX-8.f,cy-8.f,16.f,16.f},8.f,accent,.24f);
        g.setColour(accent.withAlpha(on?1.f:.82f));
        g.drawEllipse(iconX-6.f,cy-6.f,12.f,12.f,1.4f);
        g.fillEllipse(iconX-2.5f,cy-2.5f,5.f,5.f);
        g.setFont(juce::Font(juce::FontOptions(12.f,juce::Font::bold)));
        g.setColour(text.withAlpha(down?.70f:over?1.f:.94f));
        g.drawText(button.getButtonText(),juce::Rectangle<float>(iconX+13.f,r.getY(),r.getRight()-(iconX+18.f),r.getHeight()),juce::Justification::centredLeft);
        return;
    }
    juce::LookAndFeel_V4::drawButtonText(g,button,over,down);
}

void RefMatchLookAndFeel::drawLinearSlider(juce::Graphics& g,int x,int y,int width,int height,float position,float,float,juce::Slider::SliderStyle,juce::Slider& slider)
{
    const float mid=y+height*.5f;
    g.setColour(line.darker(.08f));g.fillRoundedRectangle(float(x),mid-2,float(width),4,2);
    const auto accent=slider.findColour(juce::Slider::trackColourId);
    g.setColour(juce::Colours::white.withAlpha(.025f));g.fillRoundedRectangle(float(x),mid-2,float(width),1.2f,1.f);
    g.setGradientFill(juce::ColourGradient(bool(slider.getProperties()["dualAccent"])?cyan:accent,float(x),mid,bool(slider.getProperties()["dualAccent"])?violet:accent,float(x+width),mid,false));
    g.fillRoundedRectangle(float(x),mid-2,std::max(0.f,position-x),4,2);
    g.setColour(accent.withAlpha(.12f));g.fillEllipse(position-11,mid-11,22,22);
    g.setColour(accent.withAlpha(.18f));g.fillEllipse(position-8,mid-8,16,16);
    g.setColour(text.withAlpha(.95f));g.fillEllipse(position-5.2f,mid-5.2f,10.4f,10.4f);
}

void RefMatchLookAndFeel::drawToggleButton(juce::Graphics& g,juce::ToggleButton& button,bool over,bool down)
{
    if(bool(button.getProperties()["pillToggle"])) {
        auto r=button.getLocalBounds().toFloat();
        const float h=18.f,w=36.f;
        auto pill=juce::Rectangle<float>(r.getX()+2.f,r.getCentreY()-h*.5f,w,h);
        const bool on=button.getToggleState();
        const auto accent=button.findColour(juce::ToggleButton::tickColourId);
        if(on) glowRounded(g,pill,9.f,accent,.18f);
        g.setColour(on?accent.withAlpha(.28f):line.withAlpha(.8f));g.fillRoundedRectangle(pill,9.f);
        g.setColour(on?accent.withAlpha(.80f):muted.withAlpha(.45f));g.drawRoundedRectangle(pill,9.f,1.f);
        const float cx=on?pill.getRight()-9.f:pill.getX()+9.f;
        g.setColour(on?text:muted);g.fillEllipse(cx-5.5f,pill.getCentreY()-5.5f,11.f,11.f);
        g.setFont(juce::Font(juce::FontOptions(11.f,juce::Font::bold)));
        if(bool(button.getProperties()["loopGroupToggle"]))
            g.setColour((on?accent:muted).withAlpha(down?.72f:over?1.f:(on?.96f:.86f)));
        else
            g.setColour(text.withAlpha(down?.7f:over?1.f:.92f));
        g.drawText(button.getButtonText(),juce::Rectangle<float>(pill.getRight()+8.f,r.getY(),std::max(0.f,r.getWidth()-pill.getWidth()-10.f),r.getHeight()),juce::Justification::centredLeft);
        return;
    }
    juce::LookAndFeel_V4::drawToggleButton(g,button,over,down);
}
RefMatchAudioProcessorEditor::RefMatchAudioProcessorEditor(RefMatchAudioProcessor& p):AudioProcessorEditor(&p),processor(p)
{
    setLookAndFeel(&look);setResizable(false,false);
    for(juce::Component* c:std::initializer_list<juce::Component*>{&a,&b,&switchButton,&eqTab,&loopTab,&play,&toneButton,&toneReset,&toneOn,&graphRange,&quickLoop,&matchState,&lowType,&highType,&midQ,
        &recordMix,&recordRef,&match,&reset,&autoGain,&eqOn,&gain,&amount,&smooth,&back,&forward,&timeline,&inTime,&outTime,&setIn,&setOut,&clearLoop,&zoomMinus,&zoomPlus,&loopZoom,
        &status,&mixProfile,&refProfile,&position})addAndMakeVisible(c);
    a.onClick=[this]{processor.selectSource(false);};b.onClick=[this]{processor.selectSource(true);};
    switchButton.onClick=[this]{processor.switchWithSystemMedia();};
    quickLoop.onClick=[this]{
        if(quickLoop.getToggleState()) {
            if(processor.getLoop().getOut()>processor.getLoop().getIn()) processor.getLoop().enable(true);
            else {quickLoop.setToggleState(false,juce::dontSendNotification);message="Set a loop range once in LOOP first";}
        } else processor.getLoop().enable(false);
    };
    eqTab.onClick=[this]{setPage(1);};
    // LOOP behaves like an expandable view: the main action bar stays in place,
    // and the LOOP button itself shows the active state while the loop editor is open.
    loopTab.onClick=[this]{setPage(page==2?1:2);};
    play.onClick=[this]{
        if(!processor.isReferenceSelected())processor.selectSource(true);
        else transport(processor.getMediaController().playbackState()==1?SystemMediaController::Command::pause:SystemMediaController::Command::play);
        timerCallback();
    };
    back.onClick=[this]{processor.getLoop().skip(-5);};forward.onClick=[this]{processor.getLoop().skip(5);};
    back.setButtonText("-5 s");forward.setButtonText("+5 s");
    // Keep the compact reference transport visually clean: no hover tooltip
    // can cover the waveform/player card, and the play/pause glyph is drawn
    // by the LookAndFeel rather than relying on a font glyph.
    back.setTooltip({});forward.setTooltip({});play.setTooltip({});
    play.getProperties().set("playerIcon",true);
    timeline.onRange=[this](double start,double end){
        inTime.setText(rangeText(start));outTime.setText(rangeText(end));
        if(processor.getLoop().setRange(start,end))processor.getLoop().enable(true);
    };
    timeline.onSeek=[this](double seconds){processor.getLoop().seek(seconds);};
    toneButton.onClick=[this]{showTone=true;};
    toneReset.onClick=[this]{for(int i=0;i<3;++i) {
        const auto id="tone"+juce::String(i)+"gain";
        auto* p=processor.apvts.getParameter(id);p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(0));p->endChangeGesture();
    }};
    recordMix.onClick=[this]{processor.recordProfile(LearnCapture::mix);};
    recordRef.onClick=[this]{processor.recordProfile(LearnCapture::reference);};
    match.onClick=[this]{
        if(matchAnalyzing || processor.hasMatch() || !processor.hasEnoughMatchData())return;
        matchAnalyzing=true;
        match.setButtonText("ANALYZING...");
        match.setEnabled(false);
        message="Analyzing captured tonal profiles...";
        repaint();
        juce::Component::SafePointer<RefMatchAudioProcessorEditor> safe(this);
        juce::Timer::callAfterDelay(60,[safe]{
            if(!safe)return;
            safe->processor.learnMatch();
            safe->matchFlashUntil=juce::Time::getMillisecondCounterHiRes()+850.0;
            safe->message="Match applied";
            safe->matchAnalyzing=false;
            safe->repaint();
        });
    };
    reset.onClick=[this]{processor.resetSession();message.clear();matchFlashUntil=0.0;matchReady=false;matchAnalyzing=false;nextResidualUpdateMs=0.0;lastResidualDb=-1.0f;match.setButtonText("MATCH");repaint();};
    autoGain.onClick=[this]{processor.autoGainMatch();repaint();};
    reset.getProperties().set("resetIcon",true);
    for(auto* slider:{&gain,&amount,&smooth,&midQ}) {slider->setSliderStyle(juce::Slider::LinearHorizontal);slider->setTextBoxStyle(juce::Slider::TextBoxRight,false,66,24);}
    gain.setTextValueSuffix(" dB");amount.setTextValueSuffix(" %");smooth.setTextValueSuffix(" %");midQ.setTextValueSuffix(" Q");
    midQ.setColour(juce::Slider::trackColourId,cyan.interpolatedWith(violet,.52f));
    gainAttach=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,"sourcegain",gain);
    amountAttach=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,"matchamount",amount);
    smoothAttach=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,"smooth",smooth);
    midQAttach=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,"tone1q",midQ);
    smooth.setTooltip("Fine to broad correction. Recalculates from captured profiles without recording again.");
    amount.getProperties().set("dualAccent",true);smooth.setColour(juce::Slider::trackColourId,violet);
    for(int i=0;i<6;++i) {
        addAndMakeVisible(tone[i]);tone[i].setSliderStyle(juce::Slider::LinearHorizontal);
        tone[i].setTextBoxStyle(juce::Slider::TextBoxRight,false,70,22);
        tone[i].setTextValueSuffix(i%2?" Hz":" dB");
        const auto band=i/2;
        const auto midAccent=cyan.interpolatedWith(violet,.52f);
        tone[i].setColour(juce::Slider::trackColourId,band==0?cyan:(band==1?midAccent:violet));
        toneAttachments[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.apvts,"tone"+juce::String(i/2)+(i%2?"freq":"gain"),tone[i]);
    }
    for(auto* button:{&b,&play,&recordRef})button->setColour(juce::TextButton::buttonOnColourId,violet);
    for(auto* button:{&play,&recordMix,&recordRef,&match,&reset,&autoGain})button->getProperties().set("glow",true);
    for(auto* button:{&play,&back,&forward,&reset,&loopTab,&toneReset})button->getProperties().set("softAction",true);
    autoGain.getProperties().set("autoGainPill",true);
    autoGain.getProperties().set("autoGainMeasuring",false);
    autoGain.getProperties().set("autoGainProgress",0.0f);
    match.getProperties().set("dualAccent",true);match.getProperties().set("forceDual",true);
    recordMix.getProperties().set("recordIcon",true);recordRef.getProperties().set("recordIcon",true);
    recordMix.getProperties().set("captureProgress",0.0f);
    recordRef.getProperties().set("captureProgress",0.0f);
    switchButton.getProperties().set("roundSwitch",true);
    switchButton.setButtonText("");
    switchButton.setTooltip("Switch between your mix and the system reference.");
    lowShelfAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.apvts,"tone0shelf",lowType);
    highShelfAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.apvts,"tone2shelf",highType);
    for(auto* typeButton:{&lowType,&highType}) {
        typeButton->setClickingTogglesState(true);
        typeButton->getProperties().set("compactType",true);
        typeButton->setColour(juce::TextButton::textColourOffId,text);
        typeButton->setColour(juce::TextButton::textColourOnId,text);
    }
    lowType.setColour(juce::TextButton::buttonColourId,cyan);
    lowType.setColour(juce::TextButton::buttonOnColourId,cyan);
    highType.setColour(juce::TextButton::buttonColourId,violet);
    highType.setColour(juce::TextButton::buttonOnColourId,violet);
    matchState.setClickingTogglesState(true);
    matchState.getProperties().set("compactType",true);
    matchState.getProperties().set("glow",true);
    matchState.getProperties().set("activeWhenOff",true);
    matchState.setColour(juce::TextButton::buttonColourId,cyan);
    matchState.setColour(juce::TextButton::buttonOnColourId,violet);
    matchState.onClick=[this]{
        if(auto* parameter=processor.apvts.getParameter("processingafter")) {
            const bool bypass=matchState.getToggleState();
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(bypass?0.0f:1.0f);
            parameter->endChangeGesture();
        }
    };
    matchState.setTooltip("Fast ORIGINAL / MATCHED comparison. This is not the Match EQ on/off switch.");
    quickLoop.getProperties().set("pillToggle",true);eqOn.getProperties().set("pillToggle",true);toneOn.getProperties().set("pillToggle",true);
    quickLoop.getProperties().set("loopGroupToggle",true);
    quickLoop.setColour(juce::ToggleButton::tickColourId,violet);eqOn.setColour(juce::ToggleButton::tickColourId,violet);toneOn.setColour(juce::ToggleButton::tickColourId,violet);
    quickLoop.setTooltip("Loop playback on/off. It is visually grouped with the LOOP editor button.");
    eqTab.getProperties().set("dualAccent",true);loopTab.getProperties().set("dualAccent",true);
    loopTab.getProperties().set("loopGroupButton",true);
    loopTab.getProperties().set("loopIcon",false);
    // Keep LOOP text light when the tab is active. The global active TextButton
    // text colour is dark for bright filled actions, but LOOP uses a dark panel
    // with a violet outline/glow, so dark text loses contrast.
    loopTab.setColour(juce::TextButton::textColourOnId,text);
    loopTab.setColour(juce::TextButton::textColourOffId,text);
    loopTab.setTooltip("Open the loop editor.");
    eqTab.getProperties().set("glow",true);loopTab.getProperties().set("glow",true);
    toneOnAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.apvts,"toneenabled",toneOn);
    toneOn.setTooltip("Bypass only the three Tone bands. Keeps their settings and leaves Match EQ active.");
    const auto plusMinus=juce::String::charToString(0x00B1);
    graphRange.addItem(plusMinus+"12 dB",12);graphRange.addItem(plusMinus+"24 dB",24);graphRange.addItem(plusMinus+"48 dB",48);graphRange.addItem(plusMinus+"96 dB",96);
    const int savedRange=int(p.apvts.state.getProperty("graphRange",12));
    graphScale=float(savedRange==12 || savedRange==24 || savedRange==48 || savedRange==96?savedRange:12);
    graphRange.setSelectedId(int(graphScale),juce::dontSendNotification);
    graphRange.setTooltip("Fixed graph range. Changes only when you choose a different range here.");
    graphRange.onChange=[this]{graphScale=float(graphRange.getSelectedId());processor.apvts.state.setProperty("graphRange",int(graphScale),nullptr);repaint();};
    eqOn.setButtonText("MATCH EQ");
    eqOn.setTooltip("Enable or bypass the learned Match EQ. Tone EQ is controlled separately below.");
    eqAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.apvts,"matchenabled",eqOn);
    inTime.setText(rangeText(p.getLoop().getIn()));outTime.setText(rangeText(p.getLoop().getOut()));
    inTime.onReturnKey=[this]{updateLoopRange();};outTime.onReturnKey=inTime.onReturnKey;
    inTime.onFocusLost=inTime.onReturnKey;outTime.onFocusLost=inTime.onReturnKey;
    setIn.onClick=[this]{const auto v=processor.getLoop().getPosition();if(v.valid){inTime.setText(rangeText(v.seconds));updateLoopRange();}};
    setOut.onClick=[this]{const auto v=processor.getLoop().getPosition();if(v.valid){outTime.setText(rangeText(v.seconds));updateLoopRange();}};
    clearLoop.onClick=[this]{processor.getLoop().clear();quickLoop.setToggleState(false,juce::dontSendNotification);message="Loop region cleared";repaint();};
    clearLoop.setTooltip("Remove the current In/Out loop region and turn looping off.");
    clearLoop.getProperties().set("trashIcon",true);
    zoomMinus.onClick=[this]{loopZoom.setValue(std::max(1.0,loopZoom.getValue()-.5));};
    zoomPlus.onClick=[this]{loopZoom.setValue(std::min(6.0,loopZoom.getValue()+.5));};
    loopZoom.setRange(1.0,6.0,.1);loopZoom.setValue(1.0);loopZoom.setSliderStyle(juce::Slider::LinearHorizontal);loopZoom.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
    loopZoom.onValueChange=[this]{timeline.setZoom(loopZoom.getValue());};
    label(status,11);label(mixProfile,9.5f,cyan.withAlpha(.72f));label(refProfile,9.5f,violet.withAlpha(.72f));label(position,13,text);
    addAndMakeVisible(residualStatus); residualStatus.setJustificationType(juce::Justification::centredLeft); residualStatus.setColour(juce::Label::textColourId,muted); residualStatus.setFont(juce::Font(juce::FontOptions(9.2f)));
    a.setTooltip("Listen to your mix. Pauses the active media player.");b.setTooltip("Listen to reference. Mutes MIX and sends system PLAY.");
    recordMix.setTooltip("Record the incoming MIX spectrum before EQ. Click again to finish.");
    recordRef.setTooltip("Record system-reference spectrum. Requires capture permission and host audio processing. Click again to finish.");
    autoGain.setTooltip("Secondary level-match control. Measure MIX and reference together for 5 seconds, then smoothly set A Gain to the reference loudness.");
    match.setTooltip("When MIX and REF are captured, READY TO MATCH lights up. Click to calculate EQ and enable it on MIX.");
    setPage(1);processor.startReferenceCapture();startTimerHz(15);
}
RefMatchAudioProcessorEditor::~RefMatchAudioProcessorEditor(){stopTimer();setLookAndFeel(nullptr);}
void RefMatchAudioProcessorEditor::setPage(int value)
{
    page=value;
    const bool main=page==1;

    // Keep the full action bar visible in both views. Opening LOOP should feel
    // like the section expands below the toolbar, not like navigating away to
    // a different toolbar. This also keeps MATCHED / capture state visible.
    for(auto* c:std::initializer_list<juce::Component*>{&recordMix,&recordRef,&match,&reset,&autoGain,&mixProfile,&refProfile})c->setVisible(true);
    eqOn.setVisible(false);
    for(auto* c:std::initializer_list<juce::Component*>{&amount,&smooth,&toneOn,&graphRange,&toneReset,&lowType,&highType,&midQ})c->setVisible(main);
    toneButton.setVisible(false);
    for(auto& control:tone)control.setVisible(main);

    for(auto* c:std::initializer_list<juce::Component*>{&timeline,&position,&clearLoop,&zoomMinus,&zoomPlus,&loopZoom})c->setVisible(page==2);
    for(auto* c:std::initializer_list<juce::Component*>{&inTime,&outTime,&setIn,&setOut})c->setVisible(false);

    eqTab.setVisible(false);
    loopTab.setButtonText(page==2?"CLOSE LOOP":"LOOP");
    loopTab.getProperties().set("closeLoopIcon",page==2);
    loopTab.getProperties().set("loopIcon",false);
    loopTab.setToggleState(page==2,juce::dontSendNotification);
    loopTab.setVisible(true);
    quickLoop.setVisible(true);
    eqOn.setVisible(false);

    setSize(960,page==1?650:560);
    resized();repaint();
}
void RefMatchAudioProcessorEditor::transport(SystemMediaController::Command c)
{
    if(processor.isTransportPending()||processor.getMediaController().isBusy())return;
    juce::Component::SafePointer<RefMatchAudioProcessorEditor> safe(this);
    processor.getMediaController().request(c,[safe](bool ok,SystemMediaInfo,juce::String error){if(safe && !ok)safe->message=error;});
}
juce::String RefMatchAudioProcessorEditor::timeText(double seconds)
{
    const int s=std::max(0,int(seconds));return juce::String(s/60)+":"+juce::String(s%60).paddedLeft('0',2);
}
juce::String RefMatchAudioProcessorEditor::rangeText(double seconds)
{
    const int millis=int(std::round(std::max(0.,seconds)*1000.));
    return juce::String(millis/60000)+":"+juce::String((millis%60000)/1000).paddedLeft('0',2)+"."+juce::String(millis%1000).paddedLeft('0',3);
}
double RefMatchAudioProcessorEditor::parseTime(const juce::String& input)
{
    const auto t=input.trim();
    if(t.isEmpty() || !t.containsOnly("0123456789:."))return -1;
    const auto pieces=juce::StringArray::fromTokens(t,":","");
    if(pieces.size()<1||pieces.size()>3)return -1;
    double result=0;
    for(int i=0;i<pieces.size();++i) {
        const auto part=pieces[i];
        if(part.isEmpty() || part=="." || part.retainCharacters(".").length()>1
            || (i<pieces.size()-1 && part.containsChar('.')))return -1;
        const double v=part.getDoubleValue();
        if(v<0||(i>0&&v>=60))return -1;
        result=result*60+v;
    }return result;
}

void RefMatchAudioProcessorEditor::updateLoopRange()
{
    processor.getLoop().setRange(parseTime(inTime.getText()),parseTime(outTime.getText()));
}
void RefMatchAudioProcessorEditor::timerCallback()
{
    const bool ref=processor.isReferenceSelected();a.setToggleState(!ref,juce::dontSendNotification);b.setToggleState(ref,juce::dontSendNotification);
    eqTab.setToggleState(page==1,juce::dontSendNotification);loopTab.setToggleState(page==2,juce::dontSendNotification);
    switchButton.setButtonText("");
    switchButton.getProperties().set("transportPending",processor.isTransportPending());
    const auto m=processor.profile(LearnCapture::mix),r=processor.profile(LearnCapture::reference);
    const bool recordingMix=processor.recording()==LearnCapture::mix;
    const bool recordingRef=processor.recording()==LearnCapture::reference;
    const auto captureProgress=[](const LearnCapture::Profile& p) {
        return juce::jlimit(0.0f,1.0f,float(p.seconds/RefMatchAudioProcessor::minimumMatchCaptureSeconds));
    };
    const auto mixProgress=captureProgress(m), refProgress=captureProgress(r);
    recordMix.getProperties().set("captureProgress",mixProgress);
    recordRef.getProperties().set("captureProgress",refProgress);
    recordMix.setToggleState(recordingMix,juce::dontSendNotification);
    recordRef.setToggleState(recordingRef,juce::dontSendNotification);
    if(recordingMix)
        recordMix.setButtonText(mixProgress>=.999f?"MIX READY":"LISTENING...  "+juce::String(m.seconds,1)+" s");
    else
        recordMix.setButtonText(m.seconds>=RefMatchAudioProcessor::minimumMatchCaptureSeconds?"MIX READY":"RECORD MIX");
    if(recordingRef)
        recordRef.setButtonText(refProgress>=.999f?"REF READY":"LISTENING...  "+juce::String(r.seconds,1)+" s");
    else
        recordRef.setButtonText(r.seconds>=RefMatchAudioProcessor::minimumMatchCaptureSeconds?"REF READY":"RECORD REF");
    const auto captureLabel=[](const LearnCapture::Profile& p,bool recording) {
        if(p.seconds>=RefMatchAudioProcessor::minimumMatchCaptureSeconds) {
            if(recording)return juce::String(p.seconds,1)+" s captured  ·  keep listening";
            return "CAPTURED  "+juce::String(p.seconds,1)+" s";
        }
        return juce::String(); // 0-8 s timing is already shown inside the progress button
    };
    mixProfile.setText(captureLabel(m,recordingMix),juce::dontSendNotification);
    refProfile.setText(captureLabel(r,recordingRef),juce::dontSendNotification);
    const bool matched=processor.hasMatch();
    matchReady=processor.hasEnoughMatchData()&&processor.recording()==LearnCapture::none&&!matched;
    match.setEnabled(!matchAnalyzing&&(matchReady||matched));eqOn.setEnabled(matched);
    const auto nowMs=juce::Time::getMillisecondCounterHiRes();
    if(!matchAnalyzing && nowMs>=matchFlashUntil) {
        const auto matchedLabel = juce::String("MATCHED  ") + juce::String::charToString(0x2713);
        match.setButtonText(matched ? matchedLabel : matchReady ? "READY TO MATCH" : "MATCH");
        if(message=="Match applied") message.clear();
    }
    juce::String info=page==2?processor.getLoop().getStatus():processor.getLearningStatus();
    if(page==1) {
        if(matchAnalyzing) info="Analyzing tonal profile...";
        else if(message=="Match applied") info="Match applied";
        else if(processor.recording()==LearnCapture::mix) info="Capturing mix...";
        else if(processor.recording()==LearnCapture::reference) info=processor.hasReferenceAudio()?"Capturing reference...":processor.getReferenceCaptureStatus()+" - waiting for audio";
        else if(!matched && processor.hasEnoughMatchData()) info="Ready to match";
        else if(!matched) {
            const bool mixReady=m.seconds>=RefMatchAudioProcessor::minimumMatchCaptureSeconds;
            const bool refReady=r.seconds>=RefMatchAudioProcessor::minimumMatchCaptureSeconds;
            if(mixReady && !refReady) info="Mix captured · capture reference";
            else if(refReady && !mixReady) info="Reference captured · capture mix";
            else info="Capture at least 8 s of MIX and REF";
        }
    }
    if(processor.getTransportError().isNotEmpty())info=processor.getTransportError();
    if(message.isNotEmpty() && message!="Match applied")info=message;
    status.setText(info,juce::dontSendNotification);status.setTooltip(info);
    quickLoop.setEnabled(processor.getLoop().hasRange());
    quickLoop.setToggleState(processor.getLoop().isEnabled(),juce::dontSendNotification);
    quickLoop.setButtonText(processor.getLoop().isEnabled()?"ON":"OFF");
    loopTab.setToggleState(page==2,juce::dontSendNotification);
    if(page!=2) {
        loopTab.setButtonText("LOOP");
        loopTab.getProperties().set("closeLoopIcon",false);
        loopTab.getProperties().set("loopIcon",false);
    }
    const bool processingAfter=processor.apvts.getRawParameterValue("processingafter")->load()>.5f;
    matchState.setToggleState(!processingAfter,juce::dontSendNotification);
    matchState.setButtonText(processingAfter?"HEAR MATCHED":"HEAR ORIGINAL");
    matchState.setVisible(page==1 && matched);
    if(page==1 && matched) {
        if(nowMs>=nextResidualUpdateMs) {
            lastResidualDb=processor.getResidualTonalErrorDb();
            nextResidualUpdateMs=nowMs+250.0;
        }
        const auto residualText=lastResidualDb>=0.0f?"Residual tonal error: "+juce::String(lastResidualDb,2)+" dB average":"Residual tonal error unavailable";
        match.setTooltip("Match captured tonal profiles. "+residualText);
        residualStatus.setVisible(false); // advanced metric lives in the tooltip, not the primary workflow
    } else { residualStatus.setVisible(false); nextResidualUpdateMs=0.0; }
    lowType.setButtonText(lowType.getToggleState()?"SHELF":"BELL");highType.setButtonText(highType.getToggleState()?"SHELF":"BELL");
    const bool toneEnabled=processor.apvts.getRawParameterValue("toneenabled")->load()>.5f;
    const float toneAlpha=toneEnabled?1.0f:.46f;
    for(auto& control:tone) control.setAlpha(toneAlpha);
    lowType.setAlpha(toneAlpha);highType.setAlpha(toneAlpha);midQ.setAlpha(toneAlpha);toneReset.setAlpha(toneEnabled?1.0f:.60f);
    const auto p=processor.getLoop().getPosition();position.setText(p.valid?timeText(p.seconds)+"  /  "+timeText(p.duration):"Position unavailable",juce::dontSendNotification);
    const auto currentMedia=processor.getLoop().getPosition();
    const auto referenceTooltip=(currentMedia.title.isNotEmpty()?currentMedia.title:currentMedia.track)
        +(currentMedia.artist.isNotEmpty()?"\n"+currentMedia.artist:juce::String());
    b.setTooltip(referenceTooltip);
    const auto playback=processor.getMediaController().playbackState();
    play.setToggleState(playback==1,juce::dontSendNotification);
    play.setButtonText("");
    play.setEnabled(!processor.isTransportPending() && !processor.getMediaController().isBusy());
    back.setEnabled(p.valid);forward.setEnabled(p.valid);
    if(processor.isAutoGainMatching()) {
        autoGain.getProperties().set("autoGainMeasuring",true);
        autoGain.getProperties().set("autoGainProgress",processor.getAutoGainProgress());
        autoGain.setButtonText("MEASURING");
        autoGain.setEnabled(false);
        autoGain.setToggleState(true,juce::dontSendNotification);
    } else {
        autoGain.getProperties().set("autoGainMeasuring",false);
        autoGain.getProperties().set("autoGainProgress",0.0f);
        const auto ag=processor.getAutoGainStatus();
        if (ag.startsWith("LEVEL MATCHED")) {
            // Completed state: keep the label readable and show the chosen
            // compensation inside the pill as a compact secondary value.
            const auto matchedDb = ag.fromFirstOccurrenceOf("LEVEL MATCHED", false, false).trim();
            autoGain.setButtonText(matchedDb.isNotEmpty() ? "AUTO GAIN|" + matchedDb : "AUTO GAIN");
            autoGain.setToggleState(true,juce::dontSendNotification);
        } else {
            autoGain.setToggleState(false,juce::dontSendNotification);
            if (ag.startsWith("RETRY")) autoGain.setButtonText("RETRY");
            else autoGain.setButtonText("AUTO GAIN");
        }
        autoGain.setEnabled(true);
    }
    timeline.update(p.seconds,p.duration,processor.getLoop().getIn(),processor.getLoop().getOut(),processor.getLoop().isEnabled(),p.valid,p.track);
    setIn.setEnabled(p.valid);setOut.setEnabled(p.valid);
    repaint();
}
void RefMatchAudioProcessorEditor::drawSpectrum(juce::Graphics& g,juce::Rectangle<float> r,bool eq)
{
    if(eq) glowRounded(g,r,9.f,violet,.07f);
    g.setGradientFill(juce::ColourGradient(panelRaised.withAlpha(.94f),r.getTopLeft(),panel.darker(.15f),r.getBottomRight(),false));
    g.fillRoundedRectangle(r,9.f);
    g.setColour(juce::Colours::white.withAlpha(.025f));g.drawRoundedRectangle(r.reduced(.7f),8.4f,1.f);
    auto plot=r.reduced(12,20);
    // Denser, studio-style logarithmic grid. Major divisions are brighter, while
    // intermediate frequency guides add depth without competing with the curve.
    g.setColour(line.withAlpha(.42f));
    for(int i=0;i<=8;++i){const float y=plot.getY()+plot.getHeight()*i/8.f;g.drawHorizontalLine(int(y),plot.getX(),plot.getRight());}
    const auto graphX=[&](double hz){return plot.getX()+float(std::log(hz/20.)/std::log(1000.))*plot.getWidth();};
    for(double hz:{30.,40.,50.,70.,100.,200.,300.,500.,700.,1000.,2000.,3000.,5000.,7000.,10000.,16000.,20000.}) {
        const bool major=hz==100. || hz==1000. || hz==10000.;
        g.setColour(line.withAlpha(major?.72f:.26f));
        g.drawVerticalLine(int(graphX(hz)),plot.getY(),plot.getBottom());
    }
    g.setColour(juce::Colours::white.withAlpha(.08f));
    g.drawHorizontalLine(int(plot.getCentreY()),plot.getX(),plot.getRight());
    if(eq) {
        // Live spectral metric follows the source that is actually being auditioned.
        // A uses the plug-in input / post-EQ analysers. B uses the continuously
        // captured system-reference analyser. Reference audio remains analysis-only
        // and is never routed through the plug-in output.
        if (processor.isReferenceSelected()) {
            const auto values=processor.getReferenceSpectrum();
            juce::Path spectrum;
            for(int i=0;i<180;++i) {
                const double hz=20*std::pow(1000.,i/179.);
                const int index=std::clamp(int(hz*SpectrumAnalyser::fftSize/48000.0),1,SpectrumAnalyser::bins-1);
                const float x=plot.getX()+i/179.f*plot.getWidth();
                const float norm=std::clamp((values[index]+90.f)/80.f,0.f,1.f);
                const float y=plot.getBottom()-norm*plot.getHeight();
                if(i==0)spectrum.startNewSubPath(x,y);else spectrum.lineTo(x,y);
            }
            auto specFill=spectrum;specFill.lineTo(plot.getRight(),plot.getBottom());specFill.lineTo(plot.getX(),plot.getBottom());specFill.closeSubPath();
            g.setColour(violet.withAlpha(.045f));g.fillPath(specFill);
            g.setColour(violet.withAlpha(.14f));g.strokePath(spectrum,juce::PathStrokeType(4.0f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
            g.setColour(violet.withAlpha(.46f));g.strokePath(spectrum,juce::PathStrokeType(1.25f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        } else {
            // MIX keeps the existing subtle before/after live metric.
            for(int side=0;side<2;++side) {
                const auto values=side?processor.getAfterEffectSpectrum():processor.getBeforeEffectSpectrum();
                juce::Path spectrum;
                for(int i=0;i<180;++i) {
                    const double hz=20*std::pow(1000.,i/179.);
                    const int index=std::clamp(int(hz*SpectrumAnalyser::fftSize/processor.getSampleRateForDisplay()),1,SpectrumAnalyser::bins-1);
                    const float x=plot.getX()+i/179.f*plot.getWidth();
                    const float norm=std::clamp((values[index]+90.f)/80.f,0.f,1.f);
                    const float y=plot.getBottom()-norm*plot.getHeight();
                    if(i==0)spectrum.startNewSubPath(x,y);else spectrum.lineTo(x,y);
                }
                const auto specColour=side?cyan.interpolatedWith(violet,.38f):cyan;
                auto specFill=spectrum;specFill.lineTo(plot.getRight(),plot.getBottom());specFill.lineTo(plot.getX(),plot.getBottom());specFill.closeSubPath();
                g.setColour(specColour.withAlpha(side?.035f:.045f));g.fillPath(specFill);
                g.setColour(specColour.withAlpha(side?.28f:.58f));
                g.strokePath(spectrum,juce::PathStrokeType(side?1.15f:1.35f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
            }
        }
        // While recording, overlay the cumulative learned profile. Because LearnCapture
        // averages every FFT frame, this line gradually settles and effectively
        // "freezes" into the spectral shape the matcher has understood so far.
        const auto activeCapture=processor.recording();
        if(activeCapture!=LearnCapture::none) {
            const auto learned=processor.profile(activeCapture);
            if(learned.seconds>0.0) {
                juce::Path learnedPath;
                for(int i=0;i<180;++i) {
                    const double hz=20*std::pow(1000.,i/179.);
                    const int index=std::clamp(int(hz*SpectrumAnalyser::fftSize/std::max(1.0,learned.sampleRate)),1,SpectrumAnalyser::bins-1);
                    const float x=plot.getX()+i/179.f*plot.getWidth();
                    const float norm=std::clamp((learned.db[index]+90.f)/80.f,0.f,1.f);
                    const float y=plot.getBottom()-norm*plot.getHeight();
                    if(i==0)learnedPath.startNewSubPath(x,y);else learnedPath.lineTo(x,y);
                }
                const auto c=activeCapture==LearnCapture::mix?cyan:violet;
                g.setColour(c.withAlpha(.10f));
                g.strokePath(learnedPath,juce::PathStrokeType(6.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
                g.setColour(c.withAlpha(.88f));
                g.strokePath(learnedPath,juce::PathStrokeType(1.8f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
            }
        }

        if(processor.hasMatch()) {
        const auto curve=processor.getMatchCurveDb();
        const auto fullCurve=processor.getMatchCurveDbAtAmount(1.0f);
        const float scale=graphScale;
        auto makeCurvePath=[&](const std::vector<float>& values){juce::Path path;for(size_t i=0;i<values.size();++i){const float x=plot.getX()+float(i)/float(values.size()-1)*plot.getWidth();const float y=plot.getCentreY()-std::clamp(values[i],-scale,scale)/(2*scale)*plot.getHeight();if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}return path;};
        // Dim 100% target underneath; bright white is the correction currently
        // being applied, so moving Amount visibly morphs toward/away from target.
        auto targetPath=makeCurvePath(fullCurve);
        // A translucent target-area fill makes the graph feel less flat while the
        // actual applied response remains the visual focus.
        if(!fullCurve.empty()) {
            auto targetFill=targetPath;
            targetFill.lineTo(plot.getRight(),plot.getCentreY());
            targetFill.lineTo(plot.getX(),plot.getCentreY());
            targetFill.closeSubPath();
            g.setGradientFill(juce::ColourGradient(violet.withAlpha(.08f),plot.getTopLeft(),violet.withAlpha(.015f),plot.getBottomLeft(),false));
            g.fillPath(targetFill);
        }
        g.setColour(violet.withAlpha(.11f));g.strokePath(targetPath,juce::PathStrokeType(5.5f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        g.setColour(violet.withAlpha(.34f));g.strokePath(targetPath,juce::PathStrokeType(1.25f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));

        // Draw the actually-applied response as a magnitude-sensitive colour line.
        // Near 0 dB it stays almost white; stronger corrections move through mint
        // into cyan/blue so it is immediately obvious where Match EQ is working.
        if(curve.size()>1) {
            const auto nearWhite=eqOn.getToggleState()?text:muted;
            const juce::Colour mint(0xff73e6b1), blue(0xff55a7ff);
            for(size_t i=1;i<curve.size();++i) {
                const float x1=plot.getX()+float(i-1)/float(curve.size()-1)*plot.getWidth();
                const float x2=plot.getX()+float(i)/float(curve.size()-1)*plot.getWidth();
                const float y1=plot.getCentreY()-std::clamp(curve[i-1],-scale,scale)/(2*scale)*plot.getHeight();
                const float y2=plot.getCentreY()-std::clamp(curve[i],-scale,scale)/(2*scale)*plot.getHeight();
                const float magnitude=.5f*(std::abs(curve[i-1])+std::abs(curve[i]));
                const float strength=std::clamp(magnitude/4.f,0.f,1.f);
                juce::Colour colour=nearWhite.interpolatedWith(mint,std::min(1.f,strength*1.6f));
                if(strength>.35f) colour=colour.interpolatedWith(blue,std::clamp((strength-.35f)/.65f,0.f,1.f));
                if(!eqOn.getToggleState()) colour=colour.withMultipliedAlpha(.55f);
                // soft bloom underneath each segment, then the crisp coloured line
                g.setColour(colour.withAlpha(.10f));
                g.drawLine(x1,y1,x2,y2,7.0f);
                g.setColour(colour.withAlpha(.30f));
                g.drawLine(x1,y1,x2,y2,4.2f);
                g.setColour(colour);
                g.drawLine(x1,y1,x2,y2,2.0f);
            }
        }

        }
        const float low=processor.apvts.getRawParameterValue("matchlow")->load();
        const float high=processor.apvts.getRawParameterValue("matchhigh")->load();
        const auto fx=[&](float hz){return plot.getX()+float(std::log(hz/20.f)/std::log(1000.f))*plot.getWidth();};
        const float lx=low<=20.01f?plot.getX():fx(low),hx=high>=19999.f?plot.getRight():fx(high);
        g.setColour(black.withAlpha(.48f));g.fillRect(plot.getX(),plot.getY(),std::max(0.f,lx-plot.getX()),plot.getHeight());g.fillRect(hx,plot.getY(),std::max(0.f,plot.getRight()-hx),plot.getHeight());
        g.setColour(cyan.withAlpha(.9f));g.drawVerticalLine(int(lx),plot.getY(),plot.getBottom());
        g.setColour(violet.withAlpha(.9f));g.drawVerticalLine(int(hx),plot.getY(),plot.getBottom());
        g.fillEllipse(lx-4,plot.getCentreY()-4,8,8);g.fillEllipse(hx-4,plot.getCentreY()-4,8,8);
        g.setFont(juce::Font(juce::FontOptions(9)));g.setColour(muted);
        const auto hzText=[](float hz){return hz>=1000.f?juce::String(hz/1000.f,hz<10000?1:0)+"k":juce::String(int(hz));};
        g.drawText("LOW "+hzText(low)+" Hz",juce::Rectangle<float>(lx+5,plot.getY(),76,14),juce::Justification::left);
        g.drawText("HIGH "+hzText(high)+" Hz",juce::Rectangle<float>(hx-82,plot.getY(),78,14),juce::Justification::right);
        g.setFont(juce::Font(juce::FontOptions(8.5f)));g.setColour(muted.withAlpha(.88f));
        g.drawText("+"+juce::String(graphScale,0),juce::Rectangle<float>(r.getX()-4.f,plot.getY()-3.f,30.f,14.f),juce::Justification::left);
        g.drawText("0",juce::Rectangle<float>(r.getX()-4.f,plot.getCentreY()-7.f,24.f,14.f),juce::Justification::left);
        g.drawText("-"+juce::String(graphScale,0),juce::Rectangle<float>(r.getX()-4.f,plot.getBottom()-10.f,30.f,14.f),juce::Justification::left);
        if(processor.hasMatch()) {
            const auto appliedCurve=processor.getMatchCurveDb();
            bool outside=false;for(auto db:appliedCurve)outside=outside || std::abs(db)>graphScale;
            if(outside){g.setColour(cyan.withAlpha(.78f));g.drawText("wider range available",juce::Rectangle<float>(r.getRight()-126.f,r.getY()+4.f,116.f,12.f),juce::Justification::right);}
        }
    }else{
        for(int side=0;side<2;++side){auto values=side?processor.getReferenceSpectrum():processor.getSourceSpectrum();juce::Path path;
            for(int i=0;i<180;++i){const double hz=20*std::pow(1000.,i/179.);const int index=std::clamp(int(hz*SpectrumAnalyser::fftSize/processor.getSampleRateForDisplay()),1,SpectrumAnalyser::bins-1);const float x=plot.getX()+i/179.f*plot.getWidth(),y=plot.getBottom()-std::clamp((values[index]+100)/100.f,0.f,1.f)*plot.getHeight();if(!i)path.startNewSubPath(x,y);else path.lineTo(x,y);}
            g.setColour(side?violet:cyan);g.strokePath(path,juce::PathStrokeType(1.5f));}
    }
    g.setFont(juce::Font(juce::FontOptions(9)));g.setColour(muted);for(double hz:{20.,100.,1000.,10000.,20000.}) {
        const float x=plot.getX()+float(std::log(hz/20.)/std::log(1000.))*plot.getWidth();
        g.drawText(hz<1000?juce::String(int(hz))+" Hz":juce::String(int(hz/1000))+" kHz",juce::Rectangle<float>(std::clamp(x-22,r.getX()+4,r.getRight()-48),r.getBottom()-16,44,14),juce::Justification::centred);
    }
}
void RefMatchAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(black);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff111827),0,0,black,960,500,false));
    g.fillRect(getLocalBounds());
    g.setColour(cyan.withAlpha(.022f));g.fillEllipse(-170.f,-55.f,520.f,380.f);
    g.setColour(violet.withAlpha(.024f));g.fillEllipse(650.f,-60.f,440.f,360.f);

    g.setColour(text);g.setFont(juce::Font(juce::FontOptions(24.f,juce::Font::bold)));
    g.drawText("RefMatch",44,18,180,30,juce::Justification::left);
    g.setFont(juce::Font(juce::FontOptions(10.f)));g.setColour(muted);
    g.drawText("Match your sound.",44,48,180,16,juce::Justification::left);
    g.drawText("v0.5.64    /    STREAM",744,24,150,20,juce::Justification::right);

    // Source cards
    const juce::Rectangle<float> mixCard(44,64,360,104), refCard(536,64,380,104);
    panelCard(g,mixCard,cyan,!processor.isReferenceSelected());
    panelCard(g,refCard,violet,processor.isReferenceSelected());
    g.setColour(text);g.setFont(juce::Font(juce::FontOptions(13.f,juce::Font::bold)));
    g.drawText("YOUR MIX",116,79,110,18,juce::Justification::left);
    g.setFont(juce::Font(juce::FontOptions(10.f)));g.setColour(muted);
    g.drawText(juce::String(processor.getSourcePeakDb(),1)+" dB",116,100,85,16,juce::Justification::left);
    g.drawText("Gain",116,123,42,18,juce::Justification::left);

    // Minimal live signal activity indicators. They are deliberately tiny: enough
    // to confirm that audio is present without turning the source cards into meters.
    auto drawSignalActivity=[&](float x,float y,float peakDb,juce::Colour accent) {
        const float norm=juce::jlimit(0.0f,1.0f,(peakDb+54.0f)/54.0f);
        constexpr int bars=4;
        for(int i=0;i<bars;++i) {
            const float threshold=(i+1)/float(bars);
            const float activity=juce::jlimit(0.0f,1.0f,(norm-threshold+0.28f)/0.28f);
            const float h=3.0f+float(i)*2.0f;
            g.setColour(activity>0.02f?accent.withAlpha(0.18f+0.72f*activity):muted.withAlpha(0.16f));
            g.fillRoundedRectangle(x+i*3.3f,y+(11.0f-h)*0.5f,1.7f,h,0.85f);
        }
    };
    drawSignalActivity(205.f,102.f,processor.getSourcePeakDb(),cyan);

    const auto media=processor.getLoop().getPosition();
    // Compact reference player layout: source badge, brighter artwork + metadata,
    // a tiny live activity indicator, then transport directly below the metadata.
    const juce::Rectangle<float> cover(620,76,46,46);
    // Artwork is intentionally latched per track. The now-playing API can refresh
    // artwork data repeatedly while unrelated UI meters repaint; accepting every
    // refresh can make the cover appear to pulse/blink. Only replace the cached
    // image when the actual media track changes.
    const auto artworkTrack = media.track.isNotEmpty() ? media.track : (media.title + "|" + media.artist);
    if(cachedArtworkTrack != artworkTrack) {
        cachedArtwork = {};
        cachedArtworkTrack = artworkTrack;
    }
    if(media.artwork.isValid() && !cachedArtwork.isValid()) {
        // Freeze the artwork into a fully opaque RGB snapshot. This deliberately
        // removes source alpha/transparency so changing card backgrounds, meters,
        // source selection or repaints can never show through the cover image.
        juce::Image frozen(juce::Image::RGB, 96, 96, true);
        juce::Graphics fg(frozen);
        fg.fillAll(juce::Colour(0xff0f1722));
        fg.drawImageWithin(media.artwork, 0, 0, frozen.getWidth(), frozen.getHeight(),
                           juce::RectanglePlacement::centred, false);
        cachedArtwork = frozen;
    }
    if(cachedArtwork.isValid()) {
        g.setOpacity(1.0f);
        g.drawImageWithin(cachedArtwork,620,76,46,46,juce::RectanglePlacement::centred, false);
        g.setOpacity(1.0f);
    } else {
        g.setColour(line);g.fillRoundedRectangle(cover,5.f);
        g.setColour(violet.withAlpha(.8f));g.fillEllipse(635,91,16,16);
    }

    const auto fullTitle=media.title.isNotEmpty()?media.title:"REFERENCE";
    const auto shownTitle=fullTitle.length()>18?fullTitle.substring(0,17)+"...":fullTitle;
    const auto shownArtist=media.artist.length()>20?media.artist.substring(0,19)+"...":media.artist;
    g.setColour(text);g.setFont(juce::Font(juce::FontOptions(11.5f,juce::Font::bold)));
    g.drawText(shownTitle,678,76,116,18,juce::Justification::left);
    g.setFont(juce::Font(juce::FontOptions(9.8f)));g.setColour(text.withAlpha(.78f));
    g.drawText(shownArtist,678,95,116,16,juce::Justification::left);

    // Keep the activity indicator, but integrate it into the metadata block
    // rather than leaving it floating above/beside the former waveform.
    drawSignalActivity(796.f,97.f,processor.getReferencePeakDb(),violet);

    // Shared LOOP + ON/OFF control. The two child controls sit inside one
    // rounded container with a divider, matching the three-state mockup:
    // OFF, ON, and loop-menu-open.
    {
        const float gx=page==2?664.f:676.f;
        const float gw=page==2?184.f:172.f;
        const juce::Rectangle<float> group(gx,184.f,gw,38.f);
        g.setGradientFill(juce::ColourGradient(panelRaised.withAlpha(.96f),group.getTopLeft(),
                                               panel.darker(.12f),group.getBottomRight(),false));
        g.fillRoundedRectangle(group,9.f);
        g.setColour(line.withAlpha(.82f));
        g.drawRoundedRectangle(group,9.f,1.f);
        const float dividerX=page==2?776.f:760.f;
        g.setColour(line.withAlpha(.72f));
        g.fillRoundedRectangle(dividerX,191.f,1.f,24.f,.5f);
    }

    if(page==1) {
        // Main graph card
        const juce::Rectangle<float> graphCard(44,248,872,244);
        g.setGradientFill(juce::ColourGradient(panelRaised.withAlpha(.97f),graphCard.getTopLeft(),panel.darker(.14f),graphCard.getBottomRight(),false));
        g.fillRoundedRectangle(graphCard,12.f);g.setColour(line.withAlpha(.88f));g.drawRoundedRectangle(graphCard,12.f,1.f);
        g.setColour(text);g.setFont(juce::Font(juce::FontOptions(13.f,juce::Font::bold)));g.drawText("MATCH EQ",58,260,120,20,juce::Justification::left);
        // legend
        auto dot=[&](float x,juce::Colour c,const juce::String& t,float alpha=1.0f){g.setColour(c.withAlpha(alpha));g.fillEllipse(x,269,7,7);g.setColour(text.withAlpha(.76f*alpha));g.setFont(juce::Font(juce::FontOptions(9.f)));g.drawText(t,int(x+14),263,104,18,juce::Justification::left);};
        dot(330,cyan,"Your Mix");dot(420,violet,"Reference");
        dot(514,juce::Colour(0xffffb5ff),"Matched (Applied)",processor.hasMatch()?1.0f:.28f);
        drawSpectrum(g,{58,282,844,150},true);
        const float low=processor.apvts.getRawParameterValue("matchlow")->load();
        const float high=processor.apvts.getRawParameterValue("matchhigh")->load();
        // controls row inside graph card
        g.setColour(muted);g.setFont(juce::Font(juce::FontOptions(10.f)));g.drawText("Amount",58,453,58,20,juce::Justification::left);g.drawText("Smooth",430,453,58,20,juce::Justification::left);
        g.drawText("Target Range",690,453,82,20,juce::Justification::left);
        auto smallBox=[&](float x,const juce::String& t){g.setColour(panel);g.fillRoundedRectangle(x,451,62,24,5);g.setColour(line);g.drawRoundedRectangle(x,451,62,24,5,1);g.setColour(text.withAlpha(.86f));g.setFont(juce::Font(juce::FontOptions(9.f)));g.drawText(t,int(x),451,62,24,juce::Justification::centred);};
        smallBox(774,low<1000?juce::String(int(std::round(low)))+" Hz":juce::String(low/1000.f,1)+" kHz");
        g.setColour(muted);g.drawText("-",836,451,12,24,juce::Justification::centred);
        smallBox(846,high<1000?juce::String(int(std::round(high)))+" Hz":juce::String(high/1000.f,high>=10000?0:1)+" kHz");

        // Tone EQ section, deliberately flatter and cleaner than the old cards.
        const juce::Rectangle<float> toneCard(44,508,872,112);
        const bool toneProcessing=processor.apvts.getRawParameterValue("toneenabled")->load()>.5f;
        const float toneVisual=toneProcessing?1.0f:.52f;
        g.setGradientFill(juce::ColourGradient(panelRaised.withAlpha(.93f*toneVisual),toneCard.getTopLeft(),panel.darker(.20f).withAlpha(toneVisual),toneCard.getBottomRight(),false));g.fillRoundedRectangle(toneCard,11.f);
        g.setColour(line.withAlpha(.78f*toneVisual));g.drawRoundedRectangle(toneCard,11.f,1.f);g.drawHorizontalLine(543,58,902);
        const char* names[3]={"LOW","MID","HIGH"}; const char* ranges[3]={"30 - 300 Hz","200 Hz - 6 kHz","3 - 20 kHz"};
        for(int i=0;i<3;++i){const float x=60.f+i*286.f;const auto accent=i==0?cyan:(i==1?cyan.interpolatedWith(violet,.52f):violet);g.setColour(accent.withAlpha(toneVisual));g.fillEllipse(x,554,10,10);g.setFont(juce::Font(juce::FontOptions(10.5f,juce::Font::bold)));g.drawText(names[i],int(x+18),548,52,22,juce::Justification::left);g.setFont(juce::Font(juce::FontOptions(8.8f)));g.setColour(muted.withAlpha(toneVisual));g.drawText(ranges[i],int(x+70),550,98,18,juce::Justification::left);g.drawText("Gain",int(x),576,34,18,juce::Justification::left);g.drawText("Freq",int(x),600,34,18,juce::Justification::left);if(i<2){g.setColour(line.withAlpha(.45f*toneVisual));g.drawVerticalLine(int(x+272),552,614);}}
    } else {
        // LOOP opens beneath the unchanged main toolbar. The highlighted LOOP
        // button above is the view indicator, so the editor only needs a clean
        // section title here.
        g.setColour(text);
        g.setFont(juce::Font(juce::FontOptions(14.f,juce::Font::bold)));
        g.drawText("LOOP REGION",50,232,150,22,juce::Justification::left);
        const auto& lp=processor.getLoop(); const double length=std::max(0.0,lp.getOut()-lp.getIn()); const int ms=int(std::round(length*1000.));
        const juce::String len=juce::String(ms/60000)+":"+juce::String((ms%60000)/1000).paddedLeft('0',2)+"."+juce::String(ms%1000).paddedLeft('0',3);
        g.setFont(juce::Font(juce::FontOptions(10.f)));g.setColour(muted);g.drawText("ZOOM",52,452,60,20,juce::Justification::left);g.drawText("LOOP LENGTH",720,452,100,20,juce::Justification::right);
        g.setFont(juce::Font(juce::FontOptions(17.f,juce::Font::bold)));g.setColour(text);g.drawText(len,828,448,88,26,juce::Justification::right);
    }

    if(page==1) {
        const auto now=juce::Time::getMillisecondCounterHiRes();
        if(now<matchFlashUntil) {
            const double remain=std::clamp((matchFlashUntil-now)/850.0,0.0,1.0);
            const float pulse=float(.35+.65*std::sin((1.0-remain)*juce::MathConstants<double>::pi));
            auto rr=match.getBounds().toFloat().expanded(8.f,6.f);glowRounded(g,rr,11.f,violet,.15f+.32f*pulse);g.setColour(cyan.withAlpha(.20f+.25f*pulse));g.drawRoundedRectangle(rr,11.f,1.5f);
        } else if(matchReady) {
            const float pulse=.5f+.5f*std::sin(float(now*.0065));
            auto rr=match.getBounds().toFloat().expanded(6.f,4.f);
            glowRounded(g,rr,11.f,violet,.08f+.13f*pulse);
            g.setColour(cyan.withAlpha(.10f+.12f*pulse));g.drawRoundedRectangle(rr,11.f,1.2f);
        }
    }
}

float RefMatchAudioProcessorEditor::hzToGraphX(float hz) const
{
    const float left=70.f,width=820.f;
    if(hz<=20.01f)return left;
    if(hz>=19999.f)return left+width;
    return left+float(std::log(std::clamp(hz,20.f,20000.f)/20.f)/std::log(1000.f))*width;
}
float RefMatchAudioProcessorEditor::graphXToHz(float x) const
{
    const float norm=std::clamp((x-70.f)/820.f,0.f,1.f);
    return 20.f*std::pow(1000.f,norm);
}
void RefMatchAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    if(page!=1 || e.position.y<300.f || e.position.y>420.f)return;
    const float low=processor.apvts.getRawParameterValue("matchlow")->load();
    const float high=processor.apvts.getRawParameterValue("matchhigh")->load();
    const float dl=std::abs(e.position.x-hzToGraphX(low)),dh=std::abs(e.position.x-hzToGraphX(high));
    if(std::min(dl,dh)>14.f)return;
    matchDrag=dl<dh?MatchDrag::low:MatchDrag::high;updateMatchHandle(e.position.x);
}
void RefMatchAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    if(matchDrag!=MatchDrag::none)updateMatchHandle(e.position.x);
}
void RefMatchAudioProcessorEditor::mouseUp(const juce::MouseEvent&) { matchDrag=MatchDrag::none; }
void RefMatchAudioProcessorEditor::updateMatchHandle(float x)
{
    float hz=graphXToHz(x);
    auto* lowP=dynamic_cast<juce::AudioParameterFloat*>(processor.apvts.getParameter("matchlow"));
    auto* highP=dynamic_cast<juce::AudioParameterFloat*>(processor.apvts.getParameter("matchhigh"));
    if(!lowP||!highP)return;
    const float low=processor.apvts.getRawParameterValue("matchlow")->load(),high=processor.apvts.getRawParameterValue("matchhigh")->load();
    if(matchDrag==MatchDrag::low){hz=std::min(hz,high/1.25f);hz=std::clamp(hz,20.f,1000.f);lowP->setValueNotifyingHost(lowP->convertTo0to1(hz));}
    if(matchDrag==MatchDrag::high){hz=std::max(hz,low*1.25f);hz=std::clamp(hz,1000.f,20000.f);highP->setValueNotifyingHost(highP->convertTo0to1(hz));}
    repaint();
}

void RefMatchAudioProcessorEditor::resized()
{
    // Top source cards
    a.setBounds(58,78,48,42); b.setBounds(554,78,48,42); switchButton.setBounds(437,70,66,66);
    gain.setBounds(158,112,224,30);
    autoGain.setBounds(260,78,132,22);
    // Reference transport now occupies the former waveform row, directly under
    // title/artist, so the card reads as one compact player block.
    back.setBounds(678,118,42,28);
    play.setBounds(726,118,34,28);
    forward.setBounds(766,118,42,28);

    // Main action row: all labels fit at the native 960 px width.
    recordMix.setBounds(36,184,166,38); recordRef.setBounds(214,184,166,38); match.setBounds(392,184,156,38); reset.setBounds(560,184,page==2?92:104,38);
    // Keep a consistent 12 px gap across the complete action row while
    // preserving MATCH on the same horizontal centre as the A/B switch.
    loopTab.setBounds(page==2?664:676,184,page==2?112:84,38);
    quickLoop.setBounds(page==2?780:764,184,page==2?68:84,38);
    eqOn.setBounds(0,0,0,0);
    mixProfile.setBounds(54,219,146,18); refProfile.setBounds(232,219,146,18); matchState.setBounds(410,224,116,20); residualStatus.setBounds(0,0,0,0);
    eqTab.setBounds(44,184,120,36);

    // Graph controls
    amount.setBounds(118,446,278,34); smooth.setBounds(488,446,202,34); graphRange.setBounds(820,254,82,24);

    // Tone EQ
    toneButton.setBounds(0,0,0,0); toneOn.setBounds(60,514,112,24); toneReset.setButtonText("Reset All"); toneReset.setBounds(814,514,88,24);
    tone[0].setBounds(98,572,176,24); tone[1].setBounds(98,596,176,24);
    tone[2].setBounds(384,572,176,24); tone[3].setBounds(384,596,176,24);
    tone[4].setBounds(670,572,176,24); tone[5].setBounds(670,596,176,24);
    lowType.setBounds(238,548,78,20); midQ.setBounds(508,546,126,24); highType.setBounds(808,548,78,20);

    // Loop page uses the exact same toolbar geometry as the main page.
    // The LOOP button is highlighted via its toggle state while the section is open.
    timeline.setBounds(48,258,868,170); position.setBounds(50,432,260,20); clearLoop.setBounds(800,228,116,26);
    zoomMinus.setBounds(116,456,32,28); loopZoom.setBounds(154,456,146,28); zoomPlus.setBounds(306,456,32,28);
    inTime.setBounds(0,0,0,0);setIn.setBounds(0,0,0,0);outTime.setBounds(0,0,0,0);setOut.setBounds(0,0,0,0);
    status.setBounds(46,getHeight()-26,868,20);
}

