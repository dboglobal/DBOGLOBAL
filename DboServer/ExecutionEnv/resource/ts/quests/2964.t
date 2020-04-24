CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2964;
	title = 296402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 296407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "2965;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 296401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 296408;
			gtype = 2;
			oklnk = 2;
			area = 296401;
			goal = 296404;
			sort = 296405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296402;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 296409;
				ctype = 1;
				idx = 4751210;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 17111400;
				cnt0 = 6;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 14582100;
				taid = 3;
				esctype = 0;
				idx2 = 11833300;
			}
			CDboTSActRegQInfo
			{
				cont = 296408;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4217.569824";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "3967.260010";
				m4ttip = 296417;
				area = 296401;
				goal = 296404;
				m0fz = "-4692.200195";
				m0widx = 1;
				m1ttip = 296415;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4271.169922";
				m3ttip = 296417;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4209.910156";
				m2fx = "4270.330078";
				m2ttip = 296416;
				m4widx = 1;
				m0fx = "4320.149902";
				m0ttip = 296415;
				m3pat = 2;
				m4pat = 2;
				title = 296402;
				m1fx = "3967.719971";
				m2fz = "-4683.669922";
				sort = 296405;
				m4fz = "-4211.839844";
			}
		}
	}
}

