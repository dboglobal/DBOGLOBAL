CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3704;
	title = 370402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 15;
				cct = 1;
				crt = 0;
				r = "40.000000";
				x = "-2808.510010";
				z = "-4292.740234";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 370420;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 370407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "1502102;";
			}
			CDboTSCheckClrQst
			{
				and = "3703;";
				flink = 0;
				flinknextqid = "3705;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 370409;
				ctype = 1;
				idx = 1502102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 370408;
				area = 370401;
				goal = 370404;
				m0fz = "-4292.740234";
				m0widx = 15;
				scitem = -1;
				sort = 370405;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "-2808.510010";
				m0ttip = 370415;
				rwd = 100;
				taid = 1;
				title = 370402;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 370408;
			gtype = 1;
			oklnk = 2;
			area = 370401;
			goal = 370404;
			sort = 370405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 370402;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 370414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 370401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1502102;";
			}
		}
	}
}

