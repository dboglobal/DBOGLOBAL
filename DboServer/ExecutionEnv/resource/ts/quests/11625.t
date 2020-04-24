CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11625;
	title = 1100664;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 27;
			prelnk = "9;";
			nolnk = 255;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckInNPC
			{
				idx = 1701101;
				rad = 20;
			}
		}
		CDboTSContGCond
		{
			cid = 26;
			prelnk = "15;";
			nolnk = 255;
			rm = 0;
			yeslnk = 16;

			CDboTSRcvSvrEvt
			{
				id = 16790;
			}
		}
		CDboTSContGCond
		{
			cid = 25;
			prelnk = "24;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 16840;
			}
		}
		CDboTSContGAct
		{
			cid = 24;
			elnk = 255;
			nextlnk = 25;
			prelnk = "23;23;22;21;20;19;18;17;16;";

			CDboTSActSendSvrEvt
			{
				id = 16810;
				tblidx = 11606;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 16830;
				tblidx = -1;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 26;
			prelnk = "13;12;11;10;14;14;";

			CDboTSActSendSvrEvt
			{
				id = 16780;
				tblidx = 11606;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 22;
			dt = 2;
			lilnk = 255;
			oklnk = 23;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 3;
			prelnk = "21;";
			dg = 1100697;
			gt = 0;
			oi = 8671101;
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 27;
			prelnk = "8;";

			CDboTSActEscort
			{
				start = 0;
				tblidx = 2761801;
				tsshare = 1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 23;
			dt = 2;
			lilnk = 255;
			oklnk = 24;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 1;
			prelnk = "22;";
			dg = 1100698;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 0;
			yeslnk = 9;

			CDboTSRcvSvrEvt
			{
				id = 16770;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 21;
			dt = 2;
			lilnk = 255;
			oklnk = 22;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "20;";
			dg = 1100696;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 20;
			dt = 2;
			lilnk = 255;
			oklnk = 21;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 3;
			prelnk = "19;";
			dg = 1100695;
			gt = 0;
			oi = 8671101;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "18;";
			dg = 1100694;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 18;
			dt = 2;
			lilnk = 255;
			oklnk = 19;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 3;
			prelnk = "17;";
			dg = 1100693;
			gt = 0;
			oi = 8671101;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 17;
			dt = 2;
			lilnk = 255;
			oklnk = 18;
			ot = 2;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "16;";
			dg = 1100692;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 16;
			dt = 2;
			lilnk = 255;
			oklnk = 17;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "26;";
			dg = 1100691;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1100690;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 13;
			dt = 2;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "12;";
			dg = 1100689;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 12;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "11;";
			dg = 1100688;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 12;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100687;
			gt = 0;
			oi = 1701101;
		}
		CDboTSContNarration
		{
			cancellnk = 15;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "27;";
			dg = 1100686;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "25;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "1;2;3;4;5;6;6;";

			CDboTSActEscort
			{
				start = 1;
				tblidx = 2761801;
				tsshare = 1;
				etype = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 16800;
				tblidx = -1;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100677;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 2761801;
			}
			CDboTSCheckClrQst
			{
				and = "11624;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 3;
			prelnk = "5;";
			dg = 1100683;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100682;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 1;
			prelnk = "3;";
			dg = 1100681;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100680;
			gt = 0;
			oi = -1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 1;
			prelnk = "1;";
			dg = 1100679;
			gt = 0;
			oi = 2761801;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100678;
			gt = 0;
			oi = -1;
		}
	}
}

