CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2175;
	title = 217502;

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
				conv = 217509;
				ctype = 1;
				idx = 3721201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 217508;
				gtype = 1;
				area = 217501;
				goal = 217504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 217505;
				stype = 1;
				taid = 1;
				title = 217502;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 217508;
			gtype = 1;
			oklnk = 2;
			area = 217501;
			goal = 217504;
			sort = 217505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 217502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 217514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 217501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031406;";
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
			stdiag = 217507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2176;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3721201;";
			}
		}
	}
}

