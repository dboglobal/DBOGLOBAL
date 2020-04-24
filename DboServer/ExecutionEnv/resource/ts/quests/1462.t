CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1462;
	title = 146202;

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
				conv = 146209;
				ctype = 1;
				idx = 7071106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 146208;
				gtype = 1;
				area = 146201;
				goal = 146204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 146205;
				stype = 2;
				taid = 1;
				title = 146202;
			}
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
			desc = 146214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 146201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 146208;
			gtype = 1;
			oklnk = 2;
			area = 146201;
			goal = 146204;
			sort = 146205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 146202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 146207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1542;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7071106;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

