#ifndef LCD_DEFINITIONS_H_
#define LCD_DEFINITIONS_H_

#define LCD_PORT         PORTA        /**< port for the LCD lines   */
#define LCD_DATA0_PORT   LCD_PORT     /**< port for 4bit data bit 0 */
#define LCD_DATA1_PORT   LCD_PORT     /**< port for 4bit data bit 1 */
#define LCD_DATA2_PORT   LCD_PORT     /**< port for 4bit data bit 2 */
#define LCD_DATA3_PORT   LCD_PORT     /**< port for 4bit data bit 3 */
#define LCD_DATA0_PIN    4            /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN    5            /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN    6            /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN    7            /**< pin for 4bit data bit 3  */
#define LCD_RS_PORT      PORTK        /**< port for RS line         */
#define LCD_RS_PIN       6            /**< pin  for RS line         */
#define LCD_RW_PORT      PORTK        /**< port for RW line         */
#define LCD_RW_PIN       7            /**< pin  for RW line         */
#define LCD_E_PORT       PORTJ        /**< port for Enable line     */
#define LCD_E_PIN        7            /**< pin  for Enable line     */



#endif /* LCD_DEFINITIONS_H_ */
