waybar-sway-workspace-group(5)

# NAME

waybar - sway workspace group module

# DESCRIPTION

The *workspace-group* module displays the active Sway workspace group.
If any workspace in an inactive group is urgent, the module receives the
_urgent_ style class.

# CONFIGURATION

Addressed by *sway/workspace-group*

*format*: ++
	typeof: string ++
	default: {short_name} ++
	The format, how information should be displayed.

*on-click*: ++
	typeof: string ++
	default: none ++
	If set to _toggle_, clicking runs *workspace group toggle* through Sway IPC.
	Other values are handled as normal Waybar click commands.

*disable-markup*: ++
	typeof: bool ++
	default: false ++
	If set to true, label text is not interpreted as pango markup.

# FORMAT REPLACEMENTS

*{group}*: Active workspace group name.

*{name}*: Active workspace group name.

*{short_name}*: First character of the active group, uppercased.

# EXAMPLES

```
"sway/workspace-group": {
	"format": "{short_name}",
	"on-click": "toggle"
}
```

# STYLE

- *#workspace-group*
- *#workspace-group.urgent*
