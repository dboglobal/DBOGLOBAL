CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1366;
	title = 136602;

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
				conv = 136609;
				ctype = 1;
				idx = 1654103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 136608;
				gtype = 1;
				area = 136601;
				goal = 136604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 136605;
				stype = 2;
				taid = 1;
				title = 136602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 136607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "1654103;";
			}
			CDboTSCheckClrQst
			{
				and = "1364;";
				flink = 1;
				flinknextqid = "1367;";
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
			cont = 136608;
			gtype = 1;
			oklnk = 2;
			area = 136601;
			goal = 136604;
			sort = 136605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 136602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 136614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 136601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131122;";
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

