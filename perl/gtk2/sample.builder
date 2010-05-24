<?xml version="1.0"?>
<interface>
  <requires lib="gtk+" version="2.16"/>
  <!-- interface-naming-policy project-wide -->
  <object class="GtkWindow" id="window1">
    <signal name="destroy" handler="cb_destroy"/>
    <child>
      <object class="GtkLabel" id="label1">
        <property name="visible">True</property>
        <property name="label" translatable="yes">Sample app</property>
      </object>
    </child>
  </object>
</interface>

