CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2113;
	title = 211302;

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
				conv = 211309;
				ctype = 1;
				idx = 3031215;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 211308;
				gtype = 1;
				area = 211301;
				goal = 211304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 211305;
				stype = 1;
				taid = 1;
				title = 211302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 211314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 211301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 211307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3031215;";
			}
			CDboTSCheckClrQst
			{
				and = "2112;";
				flink = 1;
				not = 0;
			}
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
			cont = 211308;
			gtype = 1;
			oklnk = 2;
			area = 211301;
			goal = 211304;
			sort = 211305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 211302;
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

