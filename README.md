# Inventory Framework

A modular, multiplayer-ready inventory framework for Unreal Engine 5.

Designed with reusable systems architecture in mind, supporting:

* replicated inventories
* shared world containers
* drag & drop UI
* stackable items
* item definitions & instances
* modular item fragments
* multiplayer-safe inventory transfers
* reusable plugin-based integration

---

## Features

### Inventory System

* Stackable items
* Configurable inventory size
* Item add/remove/move/swap support
* Shared containers (chests, storage, etc.)

### Multiplayer

* Replicated inventory data
* Multiplayer-safe item transfers
* Authority-separated UI interactions
* Shared inventory synchronization

### UI

* Reusable inventory grid widgets
* Drag & drop item support
* Slot swapping and stack merging
* Framework-agnostic UI architecture

## Architecture Overview

The framework separates:

- Item Definitions (static asset data)
- Item Instances (runtime replicated state)
- Inventory Components (storage and transfer logic)
- UI Widgets (presentation and drag & drop interactions)

This architecture allows inventories to remain modular, extensible, and multiplayer-safe.

---

## Example Project

The repository includes an example sandbox project demonstrating:

* player inventories
* shared chest containers
* replicated drag & drop interactions
* multiplayer synchronization

---

## Installation

1. Copy the plugin into your project's `Plugins/` folder
2. Regenerate project files
3. Build the project
4. Enable the plugin from the Unreal Editor if required

---

## Status

Stable initial release (v1.0.0).

The framework is actively maintained and may continue evolving with additional systems and improvements.
---

## Supported Engine Version

Unreal Engine 5.7
