CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 515;
	title = 51502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 51507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4141108;";
			}
			CDboTSCheckClrQst
			{
				and = "509;";
				flink = 1;
				flinknextqid = "523;524;";
				not = 0;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 252;
			cid = 3;
			lilnk = 255;
			ndesc0 = 51531;
			uspt = -1;
			desc = 51530;
			nid2 = 100;
			type = 0;
			ust = 0;
			idx = 4141108;
			ndesc2 = 51533;
			nid1 = 99;
			prelnk = "5;";
			ndesc1 = 51532;
			nid0 = 98;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "99;98;100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 98;
			rwdzeny = 0;
			desc = 51514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 51501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141108;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 51508;
				gtype = 1;
				area = 51501;
				goal = 51504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 51505;
				stype = 2;
				taid = 1;
				title = 51502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 99;
			rwdzeny = 0;
			desc = 51534;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 51502;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141108;";
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
			prelnk = "3;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 51534;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 51503;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 51508;
			gtype = 1;
			oklnk = 2;
			area = 51501;
			goal = 51504;
			sort = 51505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 98;
			title = 51502;
		}
	}
}

