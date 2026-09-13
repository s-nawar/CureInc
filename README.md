# 🦠 CureInc - Pandemic Management Strategy Game

**A challenging real-time strategy game where you must save humanity from a deadly global pandemic!**

Built in C using the raylib graphics library for native Windows performance.

---

## 🎯 Game Objective

**WIN:** Vaccinate 95% of the surviving population AND reduce infections below 2%

**LOSE IF:**
- Global deaths exceed 30% of population
- 6 or more regions have collapsed healthcare (20+ days overloaded)

---

## 🎮 How to Play

### Quick Start
1. Run `CureInc.exe`
2. Click "PLAY" to start
3. Manage research, scientists, and regional responses
4. Race against time to develop and distribute a cure!

### Core Gameplay

#### 💰 Resources
- **Funding:** $40 starting budget, +$8/day income
- Use funding to hire scientists, upgrade facilities, and support regions

#### 🔬 Cure Development (4 Phases)
1. **Discovery** - Identify and sequence the pathogen
2. **Trials** - Clinical safety and efficacy testing
3. **Production** - Manufacture vaccine doses
4. **Distribution** - Global vaccination rollout

#### 👨🔬 Research Systems
- **Hire Scientists ($100 each)** - Each adds +10% research speed and +0.2 doses/day production
- **Upgrade Labs (up to Level 3)** - Each level adds +15% research speed
  - Level 1: $150 | Level 2: $300 | Level 3: $450
- **Upgrade Production (up to Level 3)** - Each level adds +0.5 doses/day
  - Level 1: $200 | Level 2: $400 | Level 3: $600

#### 🌍 Regional Management (8 Regions)
Click on regions to access management options:

- **Fund Local Research ($100)** - Adds +15 research points, boosts healthcare
- **Close Borders ($100)** - Reduces virus spread by 50%, blocks imports completely
- **Monitor** - Track infection rates, deaths, and healthcare capacity

**Regions:**
- **Westeros** (Europe) - Starting outbreak
- **Essos** (Asia) - Secondary outbreak, largest population
- **Dorne** (Middle East/Spain)
- **The North** (Scotland/Scandinavia)
- **The Vale** (Alpine regions)
- **Iron Islands** (Australia/Oceania)
- **Beyond the Wall** (Antarctica)
- **Dothraki Sea** (Eurasian Steppe)

#### 🦠 Virus Mechanics
- **Mutations** occur every 15-22 days:
  - Airborne, Drug-Resistant, Stealth, Lethal
  - Cold-Adapted, Hot-Adapted, Fast-Spread, Long Incubation
- Each mutation increases infectivity (+5%) and resistance (+7%)
- Mutations reduce cure stability (-5% each, minimum 50%)
- Higher resistance slows research progress

#### 📰 Random Events
Events trigger every 5 days, affecting:
- Funding rates (Budget Cuts, Funding Surge)
- Research progress (Lab Breakthrough, Medical Miracle)
- Public trust and border controls
- Vaccine stockpiles (Supply Chain Collapse)

---

## 🎨 UI Guide

### Main Screen Tabs
- **Lab** - Hire scientists and upgrade facilities
- **Virus** - Monitor pathogen stats and mutations
- **Cure** - Track research phases and distribution

### Controls
- **TAB** - Cycle through regions
- **Click regions** - Open management panel
- **1x / 2x buttons** - Adjust game speed
- **|| button** - Pause game

### Color Indicators
- 🟢 **Green** - Clean/Safe regions
- 🟡 **Yellow** - Infected regions
- 🟠 **Orange** - Critical regions (30-60% infected)
- 🔴 **Red** - Devastated regions (60%+ infected)

---

## 💡 Strategy Tips

### Early Game (Days 1-30)
1. **Close borders immediately** on clean regions to contain the outbreak
2. **Invest in regional research** in infected regions (Westeros, Essos)
3. **Hire your first scientist** around Day 10-12
4. **Upgrade Lab Level 1** to multiply research bonuses

### Mid Game (Days 30-80)
1. **Keep hiring scientists** - they boost both research AND production
2. **Upgrade labs to Level 2-3** for maximum research multiplier
3. **Monitor hospital capacity** - overloaded hospitals double death rates
4. **React to mutations** - each one hurts cure effectiveness

### Late Game (Days 80-150)
1. **Upgrade production facilities** as soon as you reach Production phase
2. **Stockpile vaccines** - need 10 doses to start distribution
3. **Prioritize high-population regions** for vaccination
4. **Watch the win conditions** - need 95% vaccinated AND <2% infected

### Advanced Strategy
- **Border closures** are extremely cost-effective at $100
- **Regional research** provides better ROI than scientists early game
- **Scientist + Lab combos** multiply bonuses (1.1 × 1.15 = 1.265×)
- **Hospital overload** doubles death rate - prevent at all costs
- **Virus resistance** slows research by up to 60% - race against mutations!

---

## 🏗️ Building from Source

### Requirements
- GCC compiler (MinGW via w64devkit)
- raylib library installed at `C:\raylib\`

### Build Commands
```bash
build.bat
```
or
```bash
make
```

### Running
```bash
CureInc.exe
```

---

## 📊 Technical Details

- **Resolution:** 1366×768 pixels
- **Language:** C (C11 standard)
- **Graphics:** raylib (OpenGL-based)
- **Target:** Windows native
- **FPS:** Locked at 60
- **Day Length:** 2 real seconds per game day (configurable)

### Project Structure
```
src/
├── main.c      - Game loop and core logic (simplified with Bangla comments)
├── virus.c/h   - Virus simulation and mutation system
├── cure.c/h    - Vaccine research pipeline
├── region.c/h  - Regional management (8 world regions)
├── events.c/h  - Random event system
├── skills.c/h  - Skill tree/upgrade system
├── ui.c/h      - Enhanced UI with gradients and animations
└── types.h     - All data structures and constants
```

---

## 🎓 For Judges

### Game Difficulty (Improved)
The game has been significantly **harder to lose** with these changes:
- ✅ Win requires 95% vaccination (was 90%) and <2% infection (was 5%)
- ✅ Lose at 30% deaths (was 40%)
- ✅ Hospital collapse at 20 days (was 30 days)
- ✅ 6 regions collapsing causes loss (was all 8)
- ✅ Virus mutates faster (every 15-22 days instead of 20-30)
- ✅ Virus starts more infectious (+23%) and deadly (+60%)
- ✅ Research is 20% slower, funding is 20% lower
- ✅ Events occur every 5 days (was every 7)

### Code Quality
- **Simplified main.c** - Reduced from 148 to 130 lines with clear Bangla comments
- **Clean architecture** - Separated concerns (virus, cure, regions, UI, events)
- **Readable code** - Descriptive variable names and comprehensive comments
- **Bangla documentation** - Key game mechanics explained in Bengali

### UI Improvements
- ✅ Enhanced visual design with gradients, shadows, and animations
- ✅ 3D button effects with hover animations
- ✅ Progress bars with gradient fills and shine effects
- ✅ Better color coding for game states
- ✅ Clearer phase indicators and status displays
- ✅ Improved readability with text shadows

### Game Mechanics
- **Strategic depth** - Multiple viable strategies (containment vs research rush)
- **Risk management** - Balance spending between research, borders, and regional support
- **Time pressure** - Virus mutates faster, requiring quick decisions
- **Regional diversity** - Each region has unique stats and challenges
- **Random events** - Add unpredictability and force adaptation

---

## 📝 Version History

### v2.0 (September 2026)
- Made game significantly harder to lose
- Simplified codebase with Bangla comments
- Enhanced UI with modern visual effects
- Improved game balance and difficulty curve
- Better judge-friendly documentation

### v1.0 (Initial Release)
- Core pandemic management gameplay
- 4-phase cure development system
- 8 world regions with unique stats
- Virus mutation system
- Random events

---

## 🏆 Credits

**Developer:** [Your Name]
**Engine:** raylib (https://www.raylib.com)
**Language:** C
**Year:** 2026

---

## 📄 License

This project is created for educational purposes.

---

**Good luck saving the world! 🌍💉**
