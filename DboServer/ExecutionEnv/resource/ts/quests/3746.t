CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3746;
	title = 374602;

	CNtlTSGroup
	{
		gid = 0;

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
			prelnk = "100;";
			type = 1;
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
				tblidx = 374620;
			}
		}
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
				r = "50.000000";
				x = "-4687.000000";
				z = "-4953.000000";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 374609;
				ctype = 1;
				idx = 1161101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 374608;
				area = 374601;
				goal = 374604;
				m0fz = "-4953.000000";
				m0widx = 15;
				scitem = -1;
				sort = 374605;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "-4687.000000";
				m0ttip = 374615;
				rwd = 100;
				taid = 1;
				title = 374602;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 374601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1161101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 374607;
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
				npcidx = "1161101;";
			}
			CDboTSCheckClrQst
			{
				and = "3745;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 374608;
			gtype = 1;
			oklnk = 2;
			area = 374601;
			goal = 374604;
			sort = 374605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 374602;
		}
	}
}

