<template>
  <div class="collapsible">
    <div class="header" @click="isOpen = !isOpen">
      <div class="icon" :class="isOpen ? 'open' : ''"></div>
      {{ title }}
    </div>
    <div ref="contents" class="contents" :style="{ maxHeight, border, borderStyle }">
      <slot></slot>
    </div>
  </div>
</template>

<script setup lang="ts">
import CollapsibleIcon from "@img/collapsible-icon.svg";
import { watchEffect } from "vue";

defineProps<{
  title: string;
}>();

let isOpen = $ref(false);
const contents = $ref<HTMLDivElement>();

let maxHeight = $ref("");
let border = $ref("");
let borderStyle = $ref("");

const iconURL = `url(${CollapsibleIcon.src})`;

watchEffect(() => {
  maxHeight = isOpen ? contents?.scrollHeight + "px" : "";
  border = isOpen ? "2px #ddd" : "";
  borderStyle = isOpen ? "none solid solid solid" : "";
});
</script>

<style lang="postcss">
.collapsible {
  margin: 10px 0;

  .header {
    display: flex;
    flex-direction: row;
    gap: 8px;
    align-items: center;
    background-color: #ddd;
    cursor: pointer;
    transition: 0.4s;
    font-weight: bold;
    padding: 4px;

    &:hover {
      background-color: #ccc;
    }
  }

  .icon {
    width: 20px;
    height: 20px;
    background: v-bind(iconURL) center/16px 16px no-repeat;
    transition: 0.4s;

    &.open {
      transform: rotateZ(135deg);
    }
  }

  .contents {
    max-height: 0;
    overflow: hidden;
    transition: max-height 0.4s;
    padding: 0 10px;
  }
}
</style>
