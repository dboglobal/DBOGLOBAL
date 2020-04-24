CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2582;
	title = 258202;

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
				conv = 258209;
				ctype = 1;
				idx = 7073110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 258208;
				gtype = 1;
				area = 258201;
				goal = 258204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 258205;
				stype = 1;
				taid = 1;
				title = 258202;
			}
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
			cont = 258208;
			gtype = 1;
			oklnk = 2;
			area = 258201;
			goal = 258204;
			sort = 258205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 258202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 258214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 258201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2806101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 258207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2581;";
				flink = 1;
				flinknextqid = "2583;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7073110;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

