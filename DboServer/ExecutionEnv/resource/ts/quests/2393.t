CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2393;
	title = 239302;

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
				conv = 239309;
				ctype = 1;
				idx = 4072302;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 239308;
				gtype = 1;
				area = 239301;
				goal = 239304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 239305;
				stype = 1;
				taid = 1;
				title = 239302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 239307;
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
				npcidx = "4072302;";
			}
			CDboTSCheckClrQst
			{
				and = "2387;";
				flink = 1;
				flinknextqid = "2484;2468;2405;2470;";
				not = 0;
			}
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
			cont = 239308;
			gtype = 1;
			oklnk = 2;
			area = 239301;
			goal = 239304;
			sort = 239305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 239314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 239301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
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
	}
}

