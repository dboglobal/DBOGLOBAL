CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1407;
	title = 140702;

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
				conv = 140709;
				ctype = 1;
				idx = 8755301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 140708;
				gtype = 1;
				area = 140701;
				goal = 140704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 140705;
				stype = 2;
				taid = 1;
				title = 140702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 140714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 140701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2802101;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 140708;
			gtype = 1;
			oklnk = 2;
			area = 140701;
			goal = 140704;
			sort = 140705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 140702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 140707;
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
				npcidx = "8755301;";
			}
			CDboTSCheckClrQst
			{
				and = "1406;";
				flink = 1;
				flinknextqid = "1408;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

