<?php
/*-------------------------------------------------------+
| PHP-Fusion Content Management System
| Copyright (C) PHP-Fusion Inc
| https://www.php-fusion.co.uk/
+--------------------------------------------------------+
| Filename: Admins.inc
| Author: Frederick MC Chan
+--------------------------------------------------------+
| This program is released as free software under the
| Affero GPL license. You can redistribute it and/or
| modify it under the terms of this license which you
| can read by viewing the included agpl.txt or online
| at www.gnu.org/licenses/agpl.html. Removal of this
| copyright header is strictly prohibited without
| written permission from the original author(s).
+--------------------------------------------------------*/

namespace PHPFusion;

use Defender\Token;

/**
 * Class Admin
 * This class is called in templates/admin_header.php
 * Determine how to we set variables on 3rd party script
 */
class Admins {

    private static $instance = NULL;
    private static $admin_pages = [];
    private static $locale = [];
    /**
     * @var array - default section icons
     */
    public $admin_section_icons = [
        '0' => "<i class='fa fa-fw fa-dashboard'></i>",
        '1' => "<i class='fa fa-fw fa-microphone'></i>",
        '2' => "<i class='fa fa-fw fa-users'></i>",
        '3' => "<i class='fa fa-fw fa-cog'></i>",
        '4' => "<i class='fa fa-fw fa-wrench'></i>",
        '5' => "<i class='fa fa-fw fa-cubes'></i>"
    ];
    /**
     * Default core administration pages
     *
     * @var array
     */
    public $admin_page_icons = [
        'C'    => "<i class='admin-ico fa fa-fw fa-comments'></i>", // Comments
        'CP'   => "<i class='admin-ico fa fa-fw fa-leaf'></i>", // Custom page
        'IM'   => "<i class='admin-ico fa fa-fw fa-picture-o'></i>", // Images
        'PM'   => "<i class='admin-ico fa fa-fw fa-envelope-o'></i>", // private message
        'APWR' => "<i class='admin-ico fa fa-fw fa-medkit'></i>", // Admin Password Reset
        'AD'   => "<i class='admin-ico fa fa-fw fa-user-md'></i>", // Administrator
        'B'    => "<i class='admin-ico fa fa-fw fa-ban'></i>", // Blacklist
        'M'    => "<i class='admin-ico fa fa-fw fa-user'></i>", // Members
        'MI'   => "<i class='admin-ico fa fa-fw fa-barcode'></i>", // Migration tool
        'SU'   => "<i class='admin-ico fa fa-fw fa-file-o'></i>", // User Submissions
        'UF'   => "<i class='admin-ico fa fa-fw fa-table'></i>", // User Fields
        'UG'   => "<i class='admin-ico fa fa-fw fa-users'></i>", // user groups
        'UL'   => "<i class='admin-ico fa fa-fw fa-coffee'></i>", // user logs
        'SB'   => "<i class='admin-ico fa fa-fw fa-language'></i>", // Banners
        'BB'   => "<i class='admin-ico fa fa-fw fa-bold'></i>", // Bbcode
        'DB'   => "<i class='admin-ico fa fa-fw fa-history'></i>", // database backup
        'MAIL' => "<i class='admin-ico fa fa-fw fa-send'></i>", // Email templates
        'ERRO' => "<i class='admin-ico fa fa-fw fa-bug'></i>", // Error Logs
        'I'    => "<i class='admin-ico fa fa-fw fa-cubes'></i>", // Infusions
        'P'    => "<i class='admin-ico fa fa-fw fa-desktop'></i>", // Panels
        'PL'   => "<i class='admin-ico fa fa-fw fa-puzzle-piece'></i>", // Permalink
        'PI'   => "<i class='admin-ico fa fa-fw fa-info-circle'></i>", // php Info
        'ROB'  => "<i class='admin-ico fa fa-fw fa-android'></i>", // robots.txt
        'SL'   => "<i class='admin-ico fa fa-fw fa-link'></i>", // Site Links
        'SM'   => "<i class='admin-ico fa fa-fw fa-smile-o'></i>", // Smileys
        'TS'   => "<i class='admin-ico fa fa-fw fa-magic'></i>", // Theme
        'U'    => "<i class='admin-ico fa fa-fw fa-database'></i>", // Upgrade
        'LANG' => "<i class='admin-ico fa fa-fw fa-flag'></i>", // Language Settings
        'S1'   => "<i class='admin-ico fa fa-fw fa-hospital-o'></i>", // Main Settings
        'S2'   => "<i class='admin-ico fa fa-fw fa-clock-o'></i>", // Time and Date
        'S3'   => "<i class='admin-ico fa fa-fw fa-magic'></i>", // Theme Settings
        'S4'   => "<i class='admin-ico fa fa-fw fa-key'></i>", // Registration Settings
        'S6'   => "<i class='admin-ico fa fa-fw fa-gears'></i>", // Miscellaneous Settings
        'S7'   => "<i class='admin-ico fa fa-fw fa-envelope-square'></i>", // PM Settings
        'S9'   => "<i class='admin-ico fa fa-fw fa-users'></i>", // User Management
        'S10'  => "<i class='admin-ico fa fa-fw fa-arrow-circle-up'></i>", // Items Per Page
        'S12'  => "<i class='admin-ico fa fa-fw fa-shield'></i>", // Security Settings
        'FM'   => "<i class='admin-ico fa fa-fw fa-folder-open'></i>", // Fusion File Manager
    ];
    /**
     * @var array
     */
    private $admin_sections = [1 => FALSE, 2 => FALSE, 3 => FALSE, 4 => FALSE, 5 => FALSE];
    /**
     * @var array
     */
    private $admin_page_link = [];
    /**
     *    Constructor class. No Params
     */
    private $current_page = '';
    private $comment_type = [];
    private $submit_type = [];
    private $submit_link = [];
    private $link_type = [];
    private $submit_data = [];
    private $folder_permissions = [];

    public function __construct() {
        self::$locale = self::getAdminLocale();
    }

    public static function getAdminLocale() {
        if (empty(self::$locale)) {
            self::$locale = fusion_get_locale('', LOCALE.LOCALESET.'admin/main.php');
        }

        return self::$locale;
    }

    /**
     * Add instance
     *
     * @return static
     */
    public static function getInstance() {
        if (empty(self::$instance)) {
            self::$instance = new static();
        }

        return self::$instance;
    }

    /**
     * Cache the Current Field Inputs within Login session.
     *
     * @param       $form_id
     * @param       $form_type
     * @param       $item_id
     * @param array $callback_fields
     * @param int   $cache_time
     *
     * @return string
     */
    public function requestCache($form_id, $form_type, $item_id, array $callback_fields = [], $cache_time = 30000) {
        /*
        add_to_footer("<script src='".INCLUDES."jquery/confirm/jquery-confirm.min.js'></script>");
        add_to_head("<link rel='stylesheet' href='".INCLUDES."jquery/confirm/jquery-confirm.min.css'>");
        add_to_jquery("
            var input_fields = $('#$form_id').serialize();
            $.confirm({
            title: 'Restore Editing Session',
            content: 'There is a draft found for your previous editing session. Do you wish to restore?',
            buttons: {
                confirm: function () {
                    $.ajax({
                        url: '$remote',
                        type: 'post',
                        data: {
                        'fusion_token': '$token',
                        'aidlink': '".fusion_get_aidlink()."',
                        'fields': input_fields,
                        'form_id' : '$form_id',
                        'item_id':'$item_id',
                        'form_type':'$form_type',
                        'callback':'read_cache'
                        },
                        dataType: 'json',
                        success: function (e) {
                            if (e.response == 200) {
                                $.each(e.data, function(key, value) {
                                    $('#'+key).val( value );
                                });
                                setTimeout(function(e) {
                                    //$.alert('Your editing session has been restored');
                                    setTimeout( function(e) {UpdateAdminCache() }, $cache_time);
                                }, 100);
                            }
                        },
                        error: function(e) {
                        }
                   });
                },
                cancel: function () {
                    //$.alert('Canceled!');
                }
                /*somethingElse: {
                    text: 'Something else',
                    btnClass: 'btn-blue',
                    keys: ['enter', 'shift'],
                    action: function(){
                        $.alert('Something else?');
                    }
                }
            }
        });
        ");*/
        $remote = fusion_get_settings('site_path')."administration/includes/cache_update.php";
        $token = fusion_get_token($form_id, 5);
        add_to_jquery("
        function timedCacheRequest(timeout) {
            setTimeout(UpdateAdminCache, timeout);
        }
        function UpdateAdminCache(poll) {
            var input_fields = $('#$form_id').serialize();
            var ttl = '$cache_time';
            $.ajax({
            url: '$remote',
            type: 'post',
            dataType: 'html',
            data: {
                'fusion_token': '$token',
                'aidlink': '".fusion_get_aidlink()."',
                'fields': input_fields,
                'form_id' : '$form_id',
                'form_type':'$form_type',
                'item_id': '$item_id',
                'callback':'set_cache',
            },
            dataType: 'json',
            success: function (e) {
                console.log(e);
                console.log(poll);
                if (typeof poll === 'undefined') {
                    //console.log('we are starting a long poll now');
                    timedCacheRequest(ttl);
                }
            }
            });
        }
        // add abort long poll once we change fields.
        $('#".$form_id." :input').blur(function(e) {
            UpdateAdminCache(1);
        });
        ");
        if (!isset($_GET['autosave'])) {
            // not to pass prefix xhr for security reason.
            add_to_jquery("setTimeout( function(e) { UpdateAdminCache() }, $cache_time);");
        }

        $html = "";
        if (!empty($_SESSION['form_cache'][$form_id][$form_type][$item_id])) {

            $html .= "<label class='list-group-item text-normal spacer-xs p-15 m-b-10'>".self::$locale['290']." <a href='".clean_request('autosave=view', ['autosave'], FALSE)."'>".self::$locale['291']."</a></label>";

            if (isset($_GET['autosave']) && $_GET['autosave'] == 'view') {

                $html .= "<div id='rev-window'>\n";
                $html .= fusion_get_function('openside', "<h4><i class='fas fa-thumbtack m-r-10'></i>".self::$locale['292']."</h4>");
                $session = htmlspecialchars_decode($_SESSION['form_cache'][$form_id][$form_type][$item_id]);
                $session = str_replace('&#039;', "'", $session);
                parse_str($session, $data);
                unset($data['form_id']);
                unset($data['fusion_token']);

                $html .= "<dl id='restore_results' class='dl-horizontal'>\n";
                $fill_js = "";
                foreach ($data as $field_name => $value) {
                    if (isset($callback_fields[$field_name])) {
                        $html .= "<dt>".$callback_fields[$field_name]."</dt>\n";
                        $html .= "<dd class='m-b-15'><samp>";
                        $html .= nl2br(html_entity_decode($value));
                        $html .= "</samp>".form_hidden('s_'.$field_name, '', str_replace("'", '&#039;', $value))."</dd>\n";
                        $fill_js .= "
                            var c = $('#s_$field_name').val();
                            $('#".$field_name."').val(c);
                        ";
                    }
                }
                $html .= "</dl>\n";
                $html .= "<div class='text-right'>\n";
                $html .= "<button name='cancel_session' type='button' class='btn btn-default' value='cancel_session'>".self::$locale['cancel']."</button>\n";
                $html .= "<button name='fill_session' type='button' class='btn btn-primary' value='fill_session'>".self::$locale['293']."</button>\n";
                $html .= "</div>\n";
                $html .= fusion_get_function('closeside');
                $html .= "</div>\n";
                add_to_jquery("
                    $('button[name^=\"fill_session\"]').bind('click', function(e) {
                        $fill_js
                        $('#rev-window').hide();
                        UpdateAdminCache();
                    });
                    $('button[name^=\"cancel_session\"]').bind('click', function(e) {
                        $('#rev-window').hide();
                        UpdateAdminCache();
                    });
            ");
            }
        }

        return $html;
    }

    public function setAdmin() {
        if (!defined('IN_SETUP')) {
            self::$admin_pages = $this->getAdminPages();
            $this->admin_sections = array_filter(array_merge([
                0 => self::$locale['ac00'],
                1 => self::$locale['ac01'],
                2 => self::$locale['ac02'],
                3 => self::$locale['ac03'],
                4 => self::$locale['ac04'],
                5 => self::$locale['ac05'],
            ], $this->admin_sections));
            $this->admin_sections = array_values($this->admin_sections);
            $this->current_page = $this->_currentPage();
        }
    }

    /**
     * @return array
     */
    public function getAdminPages() {
        self::$admin_pages = array_filter(self::$admin_pages);
        if (empty(self::$admin_pages)) {
            $result = dbquery("SELECT * FROM ".DB_ADMIN." WHERE admin_language='".LANGUAGE."' ORDER BY admin_page DESC, admin_id ASC, admin_title ASC");
            $rows = dbrows($result);
            if ($rows) {
                while ($data = dbarray($result)) {
                    if (file_exists(ADMIN.$data['admin_link']) || file_exists(INFUSIONS.$data['admin_link'])) {
                        if (checkrights($data['admin_rights']) && $data['admin_link'] != "reserved") {
                            $data['admin_title'] = preg_replace("/&(?!(#\d+|\w+);)/", "&amp;", $data['admin_title']);
                            self::$admin_pages[$data['admin_page']][] = $data;
                        }
                    }
                }
            }
        }

        return self::$admin_pages;
    }

    /**
     * Build a return that always synchronize with the DB_ADMIN url.
     */
    public function _currentPage() {
        $path = $_SERVER['PHP_SELF'];
        if (defined('START_PAGE')) {
            $path_info = pathinfo(START_PAGE);
            if (stristr(FUSION_REQUEST, '/administration/')) {
                $path = $path_info['filename'].'.php';
            } else {
                $path = '../'.$path_info['dirname'].'/'.$path_info['filename'].'.php';
            }
        }

        return $path;
    }

    /**
     * @param $page          - 0-5 is core section pages. 6 and above are free to use.
     * @param $section_title - Section title
     * @param $icons         - Section Icons
     */
    public function addAdminSection($page, $section_title, $icons) {
        $this->admin_sections[$page] = $section_title;
        $this->admin_section_icons[$page] = $icons;
        self::$admin_pages[$page] = [];
    }

    public function setAdminBreadcrumbs() {
        BreadCrumbs::getInstance()->addBreadCrumb([
            'link'  => ADMIN.'index.php'.fusion_get_aidlink().'&amp;pagenum=0',
            'title' => self::$locale['ac10']
        ]);
        $acTab = (isset($_GET['pagenum']) && isnum($_GET['pagenum'])) ? $_GET['pagenum'] : $this->_isActive();
        if ($acTab != 0 && $acTab <= 5) {
            BreadCrumbs::getInstance()->addBreadCrumb([
                'link'  => ADMIN.fusion_get_aidlink()."&amp;pagenum=".$acTab,
                'title' => self::$locale['ac0'.$acTab]
            ]);
        }

    }

    /**
     * Determine which section is currently active.
     *
     * @return int|string
     */
    public function _isActive() {
        $active_key = 0;
        self::$admin_pages = $this->getAdminPages();
        if (empty($active_key) && !empty(self::$admin_pages)) {
            foreach (self::$admin_pages as $key => $data) {
                $link = [];
                foreach ($data as $arr => $admin_data) {
                    $link[] = $admin_data['admin_link'];
                }
                $data_link = array_flip($link);
                if (isset($data_link[$this->_currentPage()])) {
                    return $key;
                }
            }
        }

        return '0';
    }

    /**
     * @return array
     */
    public function getAdminPageIcons() {
        return $this->admin_page_icons;
    }

    /**
     * @param $rights
     * @param $icons
     */
    public function setAdminPageIcons($rights, $icons) {
        $this->admin_page_icons[$rights] = $icons;
    }

    public function getLinkType($type = NULL) {
        return ($type !== NULL ? (isset($this->link_type[$type]) ? $this->link_type[$type] : NULL) : $this->link_type);
    }

    /**
     * @param $type - link prefix
     * @param $link - link url
     */
    public function setLinkType($type, $link) {
        $this->link_type[$type] = $link;
    }

    /**
     * Get Submit Type
     *
     * @param null $type submit stype prefix
     *
     * @return array|mixed|null
     */
    public function getSubmitType($type = NULL) {
        return ($type !== NULL ? (isset($this->submit_type[$type]) ? $this->submit_type[$type] : NULL) : $this->submit_type);
    }

    /**
     * @param $type  - submissions prefix
     * @param $title - title
     */
    public function setSubmitType($type, $title) {
        $this->submit_type[$type] = $title;
    }

    public function getSubmitData($type = NULL) {
        return ($type !== NULL ? (isset($this->submit_data[$type]) ? $this->submit_data[$type] : NULL) : $this->submit_data);
    }

    /**
     * @param $type    - submissions prefix
     * @param $options - array(infusion_name, link, submit_link, submit_locale, title,admin_link)
     */
    public function setSubmitData($type, array $options = []) {
        defined(strtoupper($options['infusion_name']).'_EXIST') ? $this->submit_data[$type] = $options : NULL;
    }

    public function getSubmitLink($type = NULL) {
        return ($type !== NULL ? (isset($this->submit_link[$type]) ? $this->submit_link[$type] : NULL) : $this->submit_link);
    }

    /**
     * @param string $link Admin submission url
     * @param string $type Submissions stype prefix
     */
    public function setSubmitLink($type, $link) {
        $this->submit_link[$type] = $link;
    }

    public function getCommentType($type = NULL) {
        return ($type !== NULL ? (isset($this->comment_type[$type]) ? $this->comment_type[$type] : NULL) : $this->comment_type);
    }

    /**
     * @param $type  - comment prefix
     * @param $title - title
     */
    public function setCommentType($type, $title) {
        $this->comment_type[$type] = $title;
    }

    /**
     * @param $type - infusion_name
     */
    public function getFolderPermissions($type = NULL) {
        return ($type !== NULL ? (isset($this->folder_permissions[$type]) ? $this->folder_permissions[$type] : NULL) : $this->folder_permissions);
    }

    /**
     * @param $type    - infusion_name
     * @param $options - array(image_folder => TRUE or FALSE)
     */
    public function setFolderPermissions($type, array $options = []) {
        defined(strtoupper($type).'_EXIST') ? $this->folder_permissions[$type] = $options : NULL;
    }

    /**
     * @return array
     */
    public function getAdminPageLink() {
        return $this->admin_page_link;
    }

    /**
     * @return mixed
     */
    public function getCurrentPage() {
        return $this->current_page;
    }

    /**
     * Displays vertical collapsible administration navigation
     *
     * @param bool|FALSE $image_icon
     *
     * @return string
     */
    public function vertical_admin_nav($image_icon = FALSE) {
        $aidlink = fusion_get_aidlink();
        $admin_sections = self::getAdminSections();
        $admin_pages = self::getAdminPages();

        add_to_jquery('$("[data-toggle=collapse]").click(function () {$(this).find(".adl-drop i").toggleClass("fa-angle-left fa-angle-down");});');

        $html = "<ul id='adl' class='admin-vertical-link'>\n";

        foreach ($admin_sections as $i => $section_name) {
            $active = ((isset($_GET['pagenum']) && $_GET['pagenum'] == $i) || (!isset($_GET['pagenum']) && $this->_isActive() == $i)) ? TRUE : FALSE;

            $html .= "<li class='".($active ? 'active panel' : 'panel')."' >\n";

            if (!empty($admin_pages[$i]) && is_array($admin_pages[$i])) {
                $html .= "<a class='adl-link ".($active ? '' : 'collapsed')."' data-parent='#adl' data-toggle='collapse' href='#adl-$i'>".$this->get_admin_section_icons($i)." <span class='adl-section-name'>".$section_name.($i > 4 ? " <span class='badge'>".count($admin_pages[$i])."</span>" : "")."</span> ".($i > 0 ? "<span class='adl-drop pull-right'><i class='fa fa-angle-".($active ? "left" : "down")."'></i></span>" : '')."</a>\n";
                $html .= "<ul id='adl-$i' class='admin-submenu collapse ".($active ? 'in' : '')."'>\n";

                foreach ($admin_pages[$i] as $key => $data) {

                    $title = $data['admin_title'];
                    if ($data['admin_page'] !== 5) {
                        $title = isset(self::$locale[$data['admin_rights']]) ? self::$locale[$data['admin_rights']] : $title;
                    }

                    $secondary_active = $data['admin_link'] == $this->_currentPage() ? TRUE : FALSE;
                    $icons = ($image_icon === TRUE) ? "<img class='admin-image' src='".get_image("ac_".$data['admin_rights'])."' alt='$title'/>" : $this->get_admin_icons($data['admin_rights']);

                    $html .= checkrights($data['admin_rights']) ? "<li".($secondary_active ? " class='active'" : '')."><a href='".ADMIN.$data['admin_link'].$aidlink."'>".$icons." <span class='adl-submenu-title'>".$title."</span></a></li>\n" : "";
                }

                $html .= "</ul>\n";
            } else {
                $html .= "<a class='adl-link' href='".ADMIN."index.php".$aidlink."&amp;pagenum=0'>".$this->get_admin_section_icons($i)." <span class='adl-section-name'>".$section_name."</span> ".($i > 0 ? "<span class='adl-drop pull-right'></span>" : '')."</a>\n";
            }
            $html .= "</li>\n";
        }

        $html .= "</ul>\n";

        return $html;
    }

    /**
     * @return array
     */
    public function getAdminSections() {
        return $this->admin_sections;
    }

    /**
     * @param $page_number
     *
     * @return string
     */
    public function get_admin_section_icons($page_number) {
        if (!empty($this->admin_section_icons[$page_number]) && $this->admin_section_icons[$page_number]) {
            return $this->admin_section_icons[$page_number];
        }

        return FALSE;
    }

    /**
     * Replace admin page icons
     *
     * @param $page
     * @param $icons
     */
    public function setAdminSectionIcons($page, $icons) {
        if (isset($this->admin_section_icons[$page])) {
            $this->admin_section_icons[$page] = $icons;
        }
    }

    /**
     * Get the administration page icons
     *
     * @param $admin_rights
     *
     * @return bool
     */
    public function get_admin_icons($admin_rights) {
        // admin rights might not yield an icon & admin_icons override might not have the key.
        if (isset($this->admin_page_icons[$admin_rights]) && $this->admin_page_icons[$admin_rights]) {
            return $this->admin_page_icons[$admin_rights];
        }

        return FALSE;
    }

    /**
     * Displays horizontal administration navigation
     *
     * @param bool $icon_only
     *
     * @return string
     */
    public function horizontal_admin_nav($icon_only = FALSE) {
        $aidlink = fusion_get_aidlink();
        $html = "<ul class='admin-horizontal-link'>\n";
        foreach ($this->admin_sections as $i => $section_name) {
            $active = (isset($_GET['pagenum']) && $_GET['pagenum'] == $i || !isset($_GET['pagenum']) && $this->_isActive() == $i) ? 1 : 0;
            $admin_text = $icon_only == FALSE ? " ".$section_name : "";
            $html .= "<li ".($active ? "class='active'" : '')."><a title='".$section_name."' href='".ADMIN.$aidlink."&amp;pagenum=$i'>".$this->get_admin_section_icons($i).$admin_text."</a></li>\n";
        }
        $html .= "</ul>\n";

        return $html;
    }
}
