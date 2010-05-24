#!/usr/bin/env perl

#'http://translate.google.com/translate_t?text=hello&file=&sl=auto&tl=cs'
#<div id=result_box dir="ltr">ahoj</div>


package Google;
use strict;
use warnings;

	use LWP;
	use HTML::Entities;

	my $url;
	my $browser;
	my $lang = 'cs';
	#$lang = system('echo $LANG | cut -d _ -f 1');

	sub translate {
		my ($tl, $text) = @_;
		$browser = LWP::UserAgent->new();
		$browser->cookie_jar( {} );
		$browser->agent("Mozilla/4.0 (compatible; MSIE 5.5; Windows 98; Win 9x 4.90)");
		my $response = $browser->get( 'http://translate.google.com/translate_t',
			[ 'sl' => 'auto',
				'tl' => "$tl",
				'text' => "$text",
			]
		);
		#return decode_entities($response->content);
		return $response->content;
		#return $response->is_success;
	}

	print translate($lang, "Exploding iPhones may be a thing of the past. Researchers at Taiwan's Industrial Technology Research Institute have developed a new polymer, STOBA (that's self-terminated oligomers with hyper-branched architecture to you and me), that is added to the cathode material inside a lithium-ion battery to keep them from overheating. 'Fires or explosions in these batteries are caused by short circuits,' said Wu Hung-chun, a researcher at ITRI, explaining that even minor mishandling such as dropping the handset could result in damage causing a short circuit. 'The technology is ready for lithium-ion batteries used in electronic devices, mobile phones, laptops,' said Wu. And ITRI has started testing STOBA on electric car batteries.");
