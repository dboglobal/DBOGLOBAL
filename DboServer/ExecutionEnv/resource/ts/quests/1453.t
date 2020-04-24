CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1453;
	title = 145302;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 145308;
			gtype = 2;
			oklnk = 2;
			area = 145301;
			goal = 145304;
			sort = 145305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 145302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 145314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 145301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "5;";
				widx = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 145307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSClickNPC
			{
				npcidx = "6312104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1455;";
				not = 0;
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
				conv = 145309;
				ctype = 1;
				idx = 6312104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 145308;
				gtype = 2;
				area = 145301;
				goal = 145304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 145305;
				stype = 2;
				taid = 1;
				title = 145302;
			}
		}
	}
}

