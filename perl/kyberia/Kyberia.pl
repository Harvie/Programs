#!/usr/bin/env perl
#Kyberia client... (Harvie 2oo9)
#THX2 ||

use strict;
use warnings;

package Kyberia;
	use LWP;
	use HTML::Entities;

	our $url;
	our $browser;

	sub new { #constructor
		my ($self, $kurl) = @_;
		$url = $kurl;
    $browser = LWP::UserAgent->new( );
    $browser->cookie_jar( {} );
		$browser->timeout(15);
		return $self;
	}

	sub Login {
		my ($self, $user, $pass) = @_;
		my $response = $browser->post( $url,
			[ 'event' => 'login',
				'login' => $user,
				'password' => $pass,
				'login_type' => 'name',
			]
		);
		return $response->is_success;
	}

	sub Logout {
		my $response = $browser->post( $url,
			[ 'event' => 'logout'
			]
		);
		return $response->is_success;
	}

	sub SendMail {
		my ($self, $user, $message) = @_;
		my $response = $browser->post( $url,
			[ 'event' => 'send',
				'mail_to' => $user,
				'mail_to_type' => 'name',
				'mail_text' => $message,
			]
		);
		return $response->is_success;
	}

	sub GetNode {
		my ($self, $id) = @_;
		my $response = $browser->get("$url/id/$id/");
		return $response->content;
	}

	sub ReadMail {
		my $response = $browser->get("$url/id/2176597/");
		my @mails = split(/\n/, $response->content); $response = '';
		shift(@mails); #remove CSV header
		foreach (@mails) {
			my ($MSG_ID, $FROM, $FROM_ID, $TO, $TO_ID, $TIME, $NEW, $TEXT) = split(/;/, $_, 8); #parse CSV
			$TEXT =~ s/\\n/\n/g; decode_entities($TEXT); #unescape message
			print("#$MSG_ID $FROM --> $TO ($TIME)\n\n$TEXT\n---------------------------------\n");
		}
		return $response->is_success;
	}

	sub CheckMail {
		my $response = $browser->get("$url/ajax/check_new_mail.php");
		my @mails = split(/;/, $response->content);
		if($response->is_success) {
			my $text = "You have $mails[0] unread mails, last from $mails[1].";
			if($mails[0] < 1) { $text = "No unread mail, last mail from $mails[1]." }
			return ($mails[0], $mails[1], $text);
		}
		return 0;
	}

	return 1;
