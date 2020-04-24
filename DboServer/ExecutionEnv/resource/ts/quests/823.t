CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 823;
	title = 82302;

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
				conv = 82309;
				ctype = 1;
				idx = 3173201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 82308;
				gtype = 1;
				area = 82301;
				goal = 82304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 82305;
				stype = 1;
				taid = 1;
				title = 82302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 82307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 28;
			}
			CDboTSClickNPC
			{
				npcidx = "3173201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "824;";
				not = 0;
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
			cont = 82308;
			gtype = 1;
			oklnk = 2;
			area = 82301;
			goal = 82304;
			sort = 82305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 82302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 82314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 82301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071111;";
			}
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

