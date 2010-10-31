package Audio::Play::MPlayer;

use strict;
use warnings;
#use base qw(Class::Accessor::Fast);
use base qw(Object::Accessor);

=head1 NAME

Audio::Play::MPlayer - a frontend to play audio files using MPlayer

=head1 SYNOPSIS

    use Audio::Play::MPlayer;

    # same as Audio::Play::MPG123
    $player = Audio::Play::MPlayer->new;

    # Set speed to _2.5x_
    $player->speed(+1.5);

    $player->load( "ex-mp30.mp3" );
    print $player->title, "\n";
    $player->poll( 1 ) until $player->state == 0;

=head1 DESCRIPTION

This module acts as a frontend to an external MPlayer process started
with the C<-slave> command-line option.  The idea and interface (and
in part the code) has been taken from L<Audio::Play::MPG123>.

Please see L<Audio::Play::MPG123> for the documentation.  Take into account
that the methods:

    copyrighted
    emphasis
    error_protected
    extension
    layer
    mode
    mode_extension
    stat
    statfreq
    type
    url
    IN

have not been implemented, and that:

    jump
    tpf

work differently: C<jump> takes offsets in seconds, and C<tpf> always
returns C<1> to make it possible to write:

    $player->jump( 60 / $player->tpf );

=cut

use IPC::Open3 qw(open3);
use IO::Handle;

our $VERSION = '0.03';

# FIXME, missing
# url type layer mode mode_extension copyrighted error_protected
# emphasis extension
#__PACKAGE__->mk_accessors( qw(frame title artist album year comment genre samplerate channels bitrate extension) );
#Object::Accessor->new( qw(frame title artist album year comment genre samplerate channels bitrate extension) );

#my $bool   = $obj->mk_accessors( qw(frame title artist album year comment genre samplerate channels bitrate extension) );

sub new {
    my( $class, %args ) = @_;
    my $self = bless \%args, $class;

    $self->start_mplayer( $args{mplayerargs} );
    $self->{state} = 0;

		$self->mk_accessors( qw(frame title artist album year comment genre samplerate channels bitrate extension) );
    return $self;
}

sub DESTROY {
    my( $self ) = @_;

    $self->stop_mplayer;
}

sub start_mplayer {
    my( $self, $args ) = @_;
    my( $wr, $rd );

    my $pid = open3( $wr, $rd, $rd,
                     'mplayer', '-slave', '-idle', @{$args || []} );

    die "Can't start mplayer" unless $pid;

    $self->{pid} = $pid;
    $self->{r} = $rd;
    $self->{w} = $wr;
    $self->{r}->blocking( 0 );
    $self->{frame} = [ undef, undef, undef, undef ];
    $self->{buffer} = '';
}

sub stop_mplayer {
    my( $self ) = @_;

    return unless $self->{pid};
    $self->command( 'quit' );
    my $pid = delete $self->{pid};
    close delete $self->{r};
    close delete $self->{w};
    waitpid $pid, 0;
}

sub line {
    my( $self, $wait ) = @_;

    for(;;) {
        # append to buffer
        my $len = sysread $self->{r}, $self->{buffer}, 2048,
                          length( $self->{buffer} );
        return $1 if $self->{buffer} =~ s/^([^\n\r]*)[\r\n]+//;
        if( $wait ) {
            vec( my $rbits = '', fileno( $self->{r} ), 1 ) = 1;
            select $rbits, undef, undef, 60;
        } else {
            return;
        }
    }
}

my %info =
  ( meta_title       => [ 'get_meta_title',   'title', ],
    meta_artist      => [ 'get_meta_artist',  'artist', ],
    meta_album       => [ 'get_meta_album',   'album', ],
    meta_year        => [ 'get_meta_year',    'year', ],
    meta_comment     => [ 'get_meta_comment', 'comment', ],
    meta_genre       => [ 'get_meta_genre',   'genre', ],
    );

# AUDIO: 44100 Hz, 2 ch, s16le, 128.0 kbit/9.07% (ratio: 16000->176400)
# A:  16.5 (16.4) of 252.3 (04:12.3)  3.6%
#  =====  PAUSE  =====
sub parse {
    my( $self, $re, $wait ) = @_;

    while( my $line = $self->line( $wait ) ) {
        if( $line =~ /^A:\s+([\d\.]+)\s+\([\d\:\.]+\)\s+of\s+([\d\.]+)/ ) {
            $self->{frame}->[2] = $1;
            $self->{frame}->[3] = $2 - $1;
            # FIXME heuristic
            $self->{state} = 0 if $self->{frame}->[3] <= 0;
        } elsif( $line =~ /=====\s+PAUSE\s+=====/ ) {
            $self->{state} = 1;
        } elsif( $line =~ /^ANS_(\w+)='([^']+)'$/ ) {
            # FIXME quoting
            my( $k, $v ) = ( lc( $1 ), $2 );

            if( $info{$k} ) {
                $self->{$info{$k}->[1]} = $v;
            }
        } elsif( $line =~ /^AUDIO:\s+(\d+)/ ) {
            $self->{samplerate} = $1;
            if( $line =~ /(\d+)\s+ch/i ) {
                $self->{channels} = $1;
            }
            if( $line =~ /(\d+)\.\d+\s+kbit/i ) {
                $self->{bitrate} = $1;
            }
        } elsif( $line =~ /^Playing\s/ ) {
            $self->{$_->[1]} = undef foreach values %info;
            $self->command( $_->[0] ) foreach values %info;
        } elsif( $line =~ /^\s+(title|artist|album|year|comment|genre):\s(.*?)\s*$/i ) {
            # FIXME heuristic
            $self->{lc($1)} = $2;
        } else {
            # print STDERR $line, "\n";
        }
        return $line if $line =~ $re;
    }

    return;
}

sub poll {
    my( $self, $wait ) = @_;

    $self->parse( qr/./, $wait ); # wait for anything
    $self->parse( qr/^\0/, 0 ); # consume pending output
}

sub command {
    my( $self, $command ) = @_;

    print { $self->{w} } $command, "\n";
}

sub load {
    my( $self, $file ) = @_;

    # FIXME quoting
    $self->command( qq{loadfile "$file"} );
    $self->{state} = 2; # feeling lucky
}

sub state {
    my( $self ) = @_;

    return $self->{state};
}

# FIXME works more-or-less
sub stop {
    my( $self ) = @_;

    return if $self->{state} == 0;
    $self->pause if $self->{state} == 2;
    $self->command( 'pausing_keep seek 0.0 2' );
    $self->poll;
    $self->{state} = 0;
}

sub pause {
    my( $self ) = @_;

    return if $self->{state} == 0;
    $self->command( "pause" );
    if( $self->{state} == 2 ) {
        $self->parse( qr/=====\s+PAUSE\s+=====/, 1 );
        # try to parse metatdata command answers
        $self->poll;
    } elsif( $self->{state} == 1 ) {
        $self->{state} = 2;
    }
}

sub paused { $_[0]->{state} == 2 ? 0 : 1 }

# FIXME not like Audio::Play::MPG123
sub jump {
    my( $self, $seconds ) = @_;

    if( $seconds && $seconds =~ /^[+\-]/ ) {
        $self->command( "seek $seconds 0" );
    } else {
        $self->command( "seek $seconds 2" );
    }
}

# Sets the player speed to current speed + delta
sub speed {
    my( $self, $delta ) = @_;

    return if $self->{state} == 0;
    return if not defined $delta or $delta == 0;

    $self->command( sprintf( "speed %.2f", $delta ) );
    return;
}

# mock Audio::Play::MPG123
sub tpf { 1 }

=head1 AUTHOR

Mattia Barbon <mbarbon@cpan.org>, using ideas from
L<Audio::Play::MPG123> by Marc Lehmann <schmorp@schmorp.de>.

=head1 LICENSE

This program is free software; you can redistribute it and/or
modify it under the same terms as Perl itself.

=head1 SOURCES

The latest sources can be found on GitHub at
L<http://github.com/mbarbon/audio-play-mplayer/tree>

=head1 THANKS

Marc Lehmann <schmorp@schmorp.de> for L<Audio::Play::MPG123>.

The MP3 file used in tests has been taken from
L<http://www.shellworld.net/~davidsky/exam-mp3.htm> (test ID3 tag not
in original).

=cut

1;
