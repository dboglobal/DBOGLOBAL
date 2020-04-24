CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2402;
	title = 240202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 240209;
				ctype = 1;
				idx = 6412111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 240208;
				gtype = 1;
				area = 240201;
				goal = 240204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 240205;
				stype = 2;
				taid = 1;
				title = 240202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 240214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 240201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072302;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 240207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
			}
			CDboTSCheckClrQst
			{
				and = "2401;";
				flink = 1;
				flinknextqid = "2403;";
				not = 0;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 240208;
			gtype = 1;
			oklnk = 2;
			area = 240201;
			goal = 240204;
			sort = 240205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 240202;
		}
	}
}

