CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11604;
	title = 1100385;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 27;
			elnk = 255;
			nextlnk = 19;
			prelnk = "26;";

			CDboTSActRegQInfo
			{
				cont = 1100457;
				gtype = -1;
				area = 1100407;
				goal = 1100456;
				m0fz = "-1002.780029";
				m0widx = 16000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100452;
				rwd = 255;
				taid = 1;
				title = 1100455;
			}
		}
		CDboTSContGAct
		{
			cid = 25;
			elnk = 255;
			nextlnk = 254;
			prelnk = "24;24;";

			CDboTSActSendSvrEvt
			{
				id = 16070;
				tblidx = 11604;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 15;
			prelnk = "11;1;2;3;4;5;6;7;8;9;10;11;";

			CDboTSActWorldPlayScript
			{
				sid = 11603;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 23;
			prelnk = "22;";
			nolnk = 255;
			rm = 0;
			yeslnk = 24;

			CDboTSRcvSvrEvt
			{
				id = 16050;
			}
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 23;
			prelnk = "21;21;20;";

			CDboTSActSendSvrEvt
			{
				id = 16040;
				tblidx = 11604;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 20;
			dt = 2;
			lilnk = 255;
			oklnk = 21;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "19;";
			dg = 1100397;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "25;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 10;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 2;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "9;";
			dg = 1100395;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "10;";
			dg = 1100396;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100386;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 11603;
			}
			CDboTSClickNPC
			{
				npcidx = 6201101;
			}
		}
		CDboTSContGCond
		{
			cid = 26;
			prelnk = "16;";
			nolnk = 27;
			rm = 0;
			yeslnk = 18;

			CDboTSCheckInWorld
			{
				widx = 150000;
			}
		}
		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 19;
			prelnk = "26;";

			CDboTSActRegQInfo
			{
				cont = 1100457;
				gtype = -1;
				area = 1100407;
				goal = 1100456;
				m0fz = "-1002.780029";
				m0widx = 15000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100452;
				rwd = 255;
				taid = 1;
				title = 1100455;
			}
		}
		CDboTSContGCond
		{
			cid = 19;
			prelnk = "27;18;17;";
			nolnk = 255;
			rm = 0;
			yeslnk = 20;

			CDboTSRcvSvrEvt
			{
				id = 16030;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 25;
			cid = 24;
			dt = 2;
			lilnk = 255;
			oklnk = 25;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "23;";
			dg = 1100399;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 22;
			cid = 21;
			dt = 2;
			lilnk = 255;
			oklnk = 22;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "20;";
			dg = 1100398;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContGAct
		{
			cid = 17;
			elnk = 255;
			nextlnk = 19;
			prelnk = "16;";

			CDboTSActRegQInfo
			{
				cont = 1100457;
				gtype = -1;
				area = 1100407;
				goal = 1100456;
				m0fz = "-1002.780029";
				m0widx = 12000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "881.062012";
				m0ttip = 1100452;
				rwd = 255;
				taid = 1;
				title = 1100455;
			}
		}
		CDboTSContGCond
		{
			cid = 16;
			prelnk = "15;";
			nolnk = 26;
			rm = 0;
			yeslnk = 17;

			CDboTSCheckInWorld
			{
				widx = 120000;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "14;";

			CDboTSActWorldPlayScript
			{
				sid = 11604;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 16060;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 9;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "8;";
			dg = 1100394;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 9;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100393;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1100392;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 1100391;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 2;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1100390;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 4;
			dt = 2;
			lilnk = 255;
			oklnk = 5;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "3;";
			dg = 1100389;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1100388;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1100387;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 14;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100386;
			gt = 0;
			oi = 6201101;
		}
	}
}

