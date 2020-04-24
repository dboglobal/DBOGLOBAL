CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3020;
	title = 302002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 302014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 302001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512401;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 302008;
			gtype = 1;
			oklnk = 2;
			area = 302001;
			goal = 302004;
			sort = 302005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 302002;
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
			stdiag = 302007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSCheckClrQst
			{
				and = "3019;";
				flink = 0;
				flinknextqid = "3021;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511315;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 302009;
				ctype = 1;
				idx = 4511315;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 302008;
				area = 302001;
				goal = 302004;
				m0fz = "-5078.069824";
				m0widx = 1;
				scitem = -1;
				sort = 302005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4096.750000";
				m0ttip = 302015;
				rwd = 100;
				taid = 1;
				title = 302002;
				gtype = 1;
			}
		}
	}
}

